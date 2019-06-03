
#include "main.h"
#include "interrupts.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	if(htim -> Instance == TIM6){
		clockFlag = 1;
	}
	if(htim -> Instance == TIM7){
		timFlag = 1;
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
	tcolour touchClr;
	if(tsFlag){
		tsFlag = 0;
		touch.x = TS_State.touchX[0];
		touch.y = TS_State.touchY[0];
		touchClr = BSP_LCD_ReadPixel(touch.x, touch.y);
		if(touchClr==BUTTONCLR || touchClr==BUTTONTXTCLR){
			btn = toButton(touch.y);
			colourButton(btn, PRESSEDBUTTONCLR, PRESSEDBUTTONTXTCLR);
			personFlag = 0;
			btnLeft = 1;
			dsFlag = 1;
		}else if(touchClr==BLUECLR || touchClr==PINKCLR){
			personFlag = 1;
			btnLeft = 1;
			dsFlag = 1;
		}else if(btnLeft && touchClr==BCKGND){
			personFlag = 0;
			btnLeft = 0;
			colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			dsFlag = 0;
		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		dsFlag = 0;
		if(personFlag){
			if(touchClr==BLUECLR){
				if(touch.x<LCDXCNTR){
					printFemale(PINKCLR);
				}else{
					printMale(PINKCLR);
				}
			}else if(touchClr==PINKCLR){
				if(touch.x<LCDXCNTR){
					printFemale(BLUECLR);
				}else{
					printMale(BLUECLR);
				}
			}
		}else{
			//colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			mode = GAME;
			printFlag = 1;
			switch(btn){
			case 0:
				ai = 1;
				ai2 = 0;
				resetBoard();
				break;
			case 1:
				ai = 0;
				ai2 = 0;
				resetBoard();
				break;
			case 2:
				ai2 = 1;
				BSP_LCD_Clear(BCKGND);
				resetBoard();
				touch = chooseMove(avail,remain,allEnemies,game.player);
				playAI(touch);
				play();
			}
		}
	}
}

void checkGameTS(){
	if(tsFlag){
		tsFlag = 0;
		touch.x = toIndexX(TS_State.touchX[0]);
		touch.y = toIndexY(TS_State.touchY[0]);
		tcolour pixClr = BSP_LCD_ReadPixel(TS_State.touchX[0], TS_State.touchY[0]);
		if(touch.x>=0 && touch.y>=0 && touch.x<ROWS && touch.y<COLS){
			if(touch.x!=prev.x || touch.y!=prev.y){
				printBoard();
				selectSq(touch);
				dsFlag = 1;
				prev.x = touch.x;
				prev.y = touch.y;
			}
		}else if(pixClr==pieceClr[PINK] || pixClr==pieceClr[BLUE] || pixClr==DANGERCLR || pixClr==CLCKBKG || pixClr==CLCKFRAME){
			resetClock();
		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		printBoard();
		dsFlag = 0;
		if(board[touch.x][touch.y]==game.player+EDIF){
			play();
		}
	}
}

void checkTIM7(){
	if(timFlag){
		timFlag = 0;
		//timCount++;
		game.totalTime++;
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
	}
}

void checkTIM6(){
	if(clockFlag && mode==GAME){
		clockAn += 0.01;
		clockFlag = 0;
		if(clockAn==0){
			analogClock(CLCKBKG,LCLCKCNTRX);
			analogClock(CLCKBKG,RCLCKCNTRX);}
		if(clockAn<15){
			if(game.player == PINK)
				printCountdown(clockAn,pieceClr[PINK],LCLCKCNTRX);
			if(game.player == BLUE)
				printCountdown(clockAn,pieceClr[BLUE],RCLCKCNTRX);
			redFlag = 1;
		}else if(clockAn<20){
			if(game.player == PINK){
				if(redFlag){
					redFlag = 0;
					analogClock(DANGERCLR,LCLCKCNTRX);
				}

				printCountdown(clockAn,DANGERCLR,LCLCKCNTRX);}
			if(game.player == BLUE){
				if(redFlag){
					redFlag = 0;
					analogClock(DANGERCLR,RCLCKCNTRX);
				}
				printCountdown(clockAn,DANGERCLR,RCLCKCNTRX);
			}

		}else{
			swapPlayer();
		}
	}
}

_Bool checkPB(){
	if(pbFlag){
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

