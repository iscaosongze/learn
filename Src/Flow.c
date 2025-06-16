#include <string.h>
#include "alpaca.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "def.h"
#include "stateled.h"


void Lcm_Pattern(AgingStru *_this, uint8 p){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return;
   if(tcon->Pattern != 0){
       tcon->Pattern(p);
   }
}
void Lcm_EndFlow(AgingStru *_this){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return;    
  if(lcm->Flow->EndFlowSeq != 0){
       lcm->Flow->EndFlowSeq();
   }
}
void Lcm_PowerOff(AgingStru *_this){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return;
  if(tcon->PowerOffSeq != 0){
       tcon->PowerOffSeq();
   }  
} 
void Lcm_PowerOn(AgingStru *_this){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return;     
  if(tcon->PowerOnSeq != 0){
       tcon->PowerOnSeq();
   }  
}

void Lcm_EnterInCheckMode(AgingStru *_this){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return; 
  if(lcm->Flow->EnterInCheckMode != 0){
       lcm->Flow->EnterInCheckMode();
   }  
}

void Lcm_FlowFunc(AgingStru *_this, uint8 iflow, uint8 istep){
LcmStru   *lcm = _this->lcm;
TconStru  *tcon = _this->Tcon;    
  if(lcm ==0) return;
  if(tcon == 0) return;    
 
  if(iflow >= AGING_FLOW_MAX) return ;
  if(lcm->Flow->Flow[iflow].FlowFunc == 0) return ;  
  lcm->Flow->Flow[iflow].FlowFunc(istep);
}

/************************************************
 *
 * Flow
 *
 ************************************************
 */
 
void LcmValidCheck(AgingStru *_this){
int ai; 
LcmStru* pLcm = NULL;    
    for(ai=0; ai< AGB->TconNum; ai++){
        if(AGB->TconList[ai]->ValidCheck != 0){
            pLcm = AGB->TconList[ai]->ValidCheck();
            if(pLcm != NULL){
                _this->Tcon = AGB->TconList[ai];
                _this->lcm = pLcm;
                _this->bValid = TRUE;
                break;
            }          
        }        
    }
    if(ai == AGB->TconNum)
        _this->bValid = FALSE;
         // _this->lcm = 0;      
}  
 
void KeyFunc(AgingStru *_this){
LcmStru   *lcm = _this->lcm;
     if(_this->KeyCode != 0x0){
         switch(_this->KeyCode){
           case KEY_PWR:
                if(_this->StSys == ST_PWROFF){
                     LcmValidCheck(_this); 
                     if(_this->bValid == FALSE) {                   
                         _this->LedState = RLED_ON;
                     } else{
                         _this->LedState = GLED_ON;
                         _this->pCode = 0;
                         Lcm_PowerOn(_this);
                     }                       
                    _this->StSys = ST_CHECK;
                }else{
                     Lcm_PowerOff(_this);
                     _this->LedState = LED_ALL_OFF;
                     _this->StSys = ST_PWROFF;
                }
             break;
           case KEY_UP:
                if(_this->bValid == FALSE) break;             
                if(_this->StSys == ST_CHECK){
                     if(_this->pCode >= lcm->Flow->ChkNum-1)
                          _this->pCode = 0;
                     else
                          _this->pCode++;
                     
                     Lcm_Pattern(_this, lcm->Flow->ChkList[_this->pCode]);
                }else if(_this->StSys == ST_AGING || _this->StSys ==  ST_AGING_FINISH){
                     _this->pCode = 0;
                     Lcm_Pattern(_this, _this->lcm->Flow->ChkList[_this->pCode]);
                     Lcm_EnterInCheckMode(_this);
                     _this->LedState = GLED_ON;
                     _this->StSys = ST_CHECK;
                }
             break;
           case KEY_DWN:
                if(_this->bValid == FALSE) break; 
                if(_this->StSys == ST_CHECK){
                     if(_this->pCode == 0)
                          _this->pCode = lcm->Flow->ChkNum-1;
                     else
                          _this->pCode--;
                     Lcm_Pattern(_this, lcm->Flow->ChkList[_this->pCode]);
                }else if(_this->StSys == ST_AGING || _this->StSys ==  ST_AGING_FINISH){
                     _this->pCode = 0;
                     Lcm_Pattern(_this, _this->lcm->Flow->ChkList[_this->pCode]);
                     Lcm_EnterInCheckMode(_this);
                     _this->LedState = GLED_ON;
                     _this->StSys = ST_CHECK;
                }
             break;                    
           case KEY_MOD:
                if(_this->bValid == FALSE) break;               
                if(_this->StSys == ST_CHECK){
                     _this->m_loopDelay = 0; 
                     _this->iFlow = 0;
                     _this->iStep = AGING_IN_STEP;
                     _this->StepCount = 0;
                     _this->StSys = ST_AGING;
                }else if(_this->StSys == ST_AGING){
                     _this->iFlow++;
                     if(_this->iFlow>= lcm->Flow->TotalFlow)
                          _this->iFlow = 0;
                          _this->iStep = AGING_IN_STEP;                        
                }else if(_this->StSys == ST_AGING_FINISH){
                     _this->iFlow = 0;
                     _this->StSys = ST_AGING;
                } 
                if(_this->StSys == ST_AGING)
                     _this->LedState = GLED_SLOW_FLK;                  
             break;
         }
         _this->KeyCode = 0x0;
     }
}     
void AgingFunc(AgingStru *_this){
LcmStru   *lcm = _this->lcm;  
		if(_this->StSys == ST_AGING){            
        if(_this->iStep == AGING_IN_STEP){  
            Lcm_FlowFunc(_this, _this->iFlow, AGING_IN_STEP);
            _this->iStep = lcm->Flow->Flow[_this->iFlow].maxSteps;
            _this->StepCount = 0;
            _this->m_loopDelay = 0;
        }
        if(xTaskGetTickCount() > _this->m_loopDelay){          
            _this->StepCount++; 
            if(_this->StepCount > lcm->Flow->Flow[_this->iFlow].loopTimes){
                 _this->iStep = AGING_IN_STEP;
                 _this->iFlow++;
                 if(_this->iFlow>= lcm->Flow->TotalFlow){                     
                     Lcm_EndFlow(_this);
                     _this->LedState = GLED_QUICK_FLK;
                     _this->StSys = ST_AGING_FINISH;  
                 }      
            }else{
              _this->iStep++;
              if(_this->iStep >= lcm->Flow->Flow[_this->iFlow].maxSteps)
                  _this->iStep = 0;	 
              Lcm_FlowFunc(_this, _this->iFlow, _this->iStep);
            }
        }        
   }        
}
