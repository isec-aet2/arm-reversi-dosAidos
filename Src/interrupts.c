
#include "main.h"
#include "interrupts.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	if(htim -> Instance == TIM6){
		clockFlag = 1;
	}
	if(htim -> Instance == TIM7){
		timFlag = 1;
		if(personFlag){
			personFlag++;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		BSP_LED_Toggle(LED_GREEN);
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){
			tsFlag = 1;
		}
	}
	if(GPIO_Pin == GPIO_PIN_0){
		BSP_LED_Toggle(LED_RED);
		pbFlag = 1;
	}
}


void checkMenuTS(){
	if(tsFlag){
		tsFlag = 0;
		touch.x = TS_State.touchX[0];
		touch.y = TS_State.touchY[0];
		thisSide = touch.x<LCDXCNTR ? LEFT : RIGHT;
		touchClr = BSP_LCD_ReadPixel(touch.x, touch.y);
		switch(touchClr){
		case PINKCLR:
			iClr = PINK;
			break;
		case BLUECLR:
			iClr = BLUE;
		}
		if(touchClr==BUTTONCLR || touchClr==BUTTONTXTCLR){
			btn = toButton(touch.y);
			colourButton(btn, PRESSEDBUTTONCLR, PRESSEDBUTTONTXTCLR);
			personFlag = 0;
			btnLeft = 1;
			dsFlag = 1;
		}else if(touchClr==PINKCLR || touchClr==BLUECLR){
			personFlag = 1;
			btnLeft = 1;
			dsFlag = 1;
			printBody(pieceClr[iClr+AVAILDIF], thisSide, bodyDisp[thisSide]);
		}else if(btnLeft && touchClr==BCKGND){
			personFlag = 0;
			btnLeft = 0;
			colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			printBody(pieceClr[iClr], thisSide, bodyDisp[thisSide]);
			dsFlag = 0;
		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		dsFlag = 0;
		btnLeft = 0;
		printBody(pieceClr[iClr], thisSide, bodyDisp[thisSide]);
		if(personFlag>TRANSSEC){
			printBody(BCKGND, thisSide, bodyDisp[thisSide]);
			bodyDisp[thisSide] = !bodyDisp[thisSide];
			printBody(pieceClr[iClr], thisSide, bodyDisp[thisSide]);
		}else if(!personFlag){
			//colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			mode = GAME;
			printFlag = 1;
			switch(btn){
			case 0:
				aiFlag = 1;
				ai2 = 0;
				iAI = rand()%2;
				break;
			case 1:
				aiFlag = 0;
				ai2 = 0;
				break;
			case 2:
				aiFlag = 1;
				ai2 = 1;
				BSP_LCD_Clear(BCKGND);
				touch = chooseMove(avail,remain,targets,game.player);
				playAI(touch);
				play();
			}
		}else{
			printBody(pieceClr[!iClr], thisSide, bodyDisp[thisSide]);
		}
	}
}

_Bool checkGameTS(){
	if(tsFlag){
		tsFlag = 0;
		touch.x = toIndexX(TS_State.touchX[0]);
		touch.y = toIndexY(TS_State.touchY[0]);
		//touchClr = BSP_LCD_ReadPixel(TS_State.touchX[0], TS_State.touchY[0]);
		if(touch.x>=0 && touch.y>=0 && touch.x<ROWS && touch.y<COLS){
			if(touch.x!=prev.x || touch.y!=prev.y){
				deselectSq(prev);
				printFrame();
				selectSq(touch);
				dsFlag = 1;
				prev.x = touch.x;
				prev.y = touch.y;
			}
		}
//		}else if(touchClr==pieceClr[PINK] || touchClr==pieceClr[BLUE] || touchClr==DANGERCLR || touchClr==CLCKBKG || touchClr==CLCKFRAME){
//			resetClocks();
//		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		printBoard();
		dsFlag = 0;
		if(board[touch.x][touch.y]==game.player+AVAILDIF){
			return 1;
		}
	}
	return 0;
}

void checkEndTS(){
	if(tsFlag){
		tsFlag = 0;
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		dsFlag = 0;
		mode = MENU;
		menuSize = ORIGOPT;
		printFlag = 1;
	}
}

void checkTIM7(){
	if(timFlag){
		timFlag = 0;
		//timCount++;
		game.totalTime++;
		game.playerTime[game.player]++;
		if(game.totalTime%2){
			HAL_StatusTypeDef status=HAL_ADC_PollForConversion(&hadc1,TEMP_REFRESH_PERIOD);
			if(status==HAL_OK){
				convertedValue=HAL_ADC_GetValue(&hadc1);
				degrees = ((((convertedValue * VREF)/MAX_CONVERTED_VALUE) - VSENS_AT_AMBIENT_TEMP) * 10 / AVG_SLOPE) + AMBIENT_TEMP;
				sprintf(temp, "%ld degrees Celsius", degrees);
				BSP_LCD_SetTextColor(TEMPCLR);
				BSP_LCD_SetBackColor(BCKGND);
				BSP_LCD_SetFont(&TEMPFONT);
				BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)temp, RIGHT_MODE);
			}
		}
		if(mode==GAME){
			fillInfo();
			printInfo(0,0);
		}
	}
}

void checkTIM6(){
	if(clockFlag && mode==GAME){
		clockAn += 0.01;
		clockFlag = 0;
		if(clockAn==0){
			resetClocks();
		}
		if(clockAn<15){
			printCountdown(clockAn,pieceClr[game.player],(tside)game.player);
			redFlag = 1;
		}else if(clockAn<20){
			if(redFlag){
				redFlag = 0;
				analogClock(DANGERCLR,(tside)game.player);
			}
			printCountdown(clockAn,DANGERCLR,(tside)game.player);
		}else{
			if(game.nTimeOut[game.player]<TIMEOUTMAX){
				game.nTimeOut[game.player]++;
			}else{
				endGame(!game.player);
			}
			swapPlayer();
		}
	}
}

_Bool checkPB(){
	if(pbFlag && mode==GAME){
		pbFlag = 0;
//		mode = MENU;
//		printFlag = 1;
//		if(newGame){
//			menuSize = ORIGOPT+1;
//		}
		swapPlayer();
		return TRUE;
	}
	return FALSE;
}

