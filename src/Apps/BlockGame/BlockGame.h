#ifndef BLOCKGAME_APP_H
#define BLOCKGAME_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class Piece{
    public:
        int x[4]; //0-9
        int y[4]; //0-19
        int shape;
        int rotation;

        void PushDown(){
            for(int i =0; i<4;i++){
                y[i]++;
            }
        }
        void MoveLeft(){
            for(int i =0 ;i<4;i++){
                x[i]--;
            }
        }
        void MoveRight(){
            for(int i =0 ;i<4;i++){
                x[i]++;
            }
        }
        void RotateRight(){
            if(shape==0){

            }
            else if(shape==1) 
            {
                if(rotation==0)
                {
                    for(int i=0;i<4;i++){
                        x[i] = x[2];
                    }
                    y[0]+=2;
                    y[1]+=1;
                   //y[2]+=0;
                    y[3]-=1;
                }
                else if (rotation==1)
                {
                    for(int i=0;i<4;i++){
                        y[i] = y[1];
                    }
                    x[0]+=1;
                   // x[1]+=0;
                    x[2]-=1;
                    x[3]-=2;

                    
                }
                else if(rotation==2)
                {
                    for(int i=0;i<4;i++){
                        x[i] = x[2];
                    }
                    y[0]+=1;
                    //y[1]+=0;
                   y[2]-=1;
                    y[3]-=2;
                }
                else if(rotation==3)
                {
                    for(int i=0;i<4;i++){
                        y[i] = y[2];
                    }
                    x[0]-=1;
                    //y[1]+=0;
                    x[2]+=1;
                    x[3]+=2;
                }


                
                
            }
            else if(shape==2) 
            {
                if(rotation==0)
                {
                    
                    y[1]+=1;
                    y[3]+=1;
                    

                    x[0]+=2;
                    x[1]+=1;
                    x[3]-=1;

                    
                }
                else if (rotation==1)
                {
                    y[0]+=2;
                    y[1]+=1;
                    y[3]-=1;
                    
                    x[1]-=1;
                    x[3]-=1;

                    
                }
                else if(rotation==2)
                {
                    
                    y[1]-=1;
                    y[3]-=1;

                    x[0]-=2;
                    x[1]-=1;
                    x[3]+=1;
                }
                else if(rotation==3)
                {
                    y[0]-=2;
                    y[1]-=1;
                    y[3]+=1;
                    
                    x[1]+=1;
                    x[3]+=1;
                }


               
                
            }
            else if(shape==3) 
            {
                if(rotation==0)
                {
                    
                    y[0]+=2;
                    y[1]+=1;
                    y[3]-=1;
                    
                    x[1]+=1;
                    x[3]+=1;

                    
                }
                else if (rotation==1)
                {
                    
                    y[1]+=1;
                    y[3]+=1;
                    
                    x[0]-=2;
                    x[1]-=1;
                    x[3]+=1;

                    
                }
                else if(rotation==2)
                {
                    y[0]-=2;
                    y[1]-=1;
                    y[3]+=1;
                    
                    x[1]-=1;
                    x[3]-=1;
                }
                else if(rotation==3)
                {
                   
                    
                    y[1]-=1;
                    y[3]-=1;

                    x[0]+=2;
                    x[1]+=1;
                    x[3]-=1;
                }


                
                
            }
            else if(shape==4) 
            {
                if(rotation==0)
                {
                    
                    y[0]-=1;
                    y[2]+=1;
                    y[3]+=1;
                    
                    x[0]+=1;
                    x[2]-=1;
                    x[3]+=1;

                    
                }
                else if (rotation==1)
                {
                    y[0]+=1;
                    y[2]-=1;
                    y[3]+=1;
                    
                    x[0]+=1;
                    x[2]-=1;
                    x[3]-=1;

                    
                }
                else if(rotation==2)
                {
                    y[0]+=1;
                    y[2]-=1;
                    y[3]-=1;

                    
                    x[0]-=1;
                    x[2]+=1;
                    x[3]-=1;
                }
                else if(rotation==3)
                {
                    y[0]-=1;
                    y[2]+=1;
                    y[3]-=1;
                    
                    x[0]-=1;
                    x[2]+=1;
                    x[3]+=1;
                }


                
                
            }
            else if(shape==5) 
            {
                if(rotation==0)
                {
                    
                    y[0]-=1;
                    y[2]+=1;
                    y[3]+=2;

                    x[0]+=1;
                    x[2]-=1;

                    
                }
                else if (rotation==1)
                {
                    y[0]+=1;
                    y[2]-=1;
                    
                    x[0]+=1;
                    x[2]-=1;
                    x[3]-=2;

                    
                }
                else if(rotation==2)
                {
                    y[0]+=1;
                    y[2]-=1;
                    y[3]-=2;
                    
                    x[0]-=1;
                    x[2]+=1;
                    
                }
                else if(rotation==3)
                {
                    y[0]-=1;
                    y[2]+=1;
                    
                    x[0]-=1;
                    x[2]+=1;
                    x[3]+=2;
                }


                
                
            }
            else if(shape==6) 
            {
                if(rotation==0)
                {
                    
                    y[0]-=1;
                    y[2]+=1;
                    
                    x[0]+=1;
                    x[2]-=1;
                    x[3]+=2;


                    
                }
                else if (rotation==1)
                {
                    y[0]+=1;
                    y[2]-=1;
                    y[3]+=2;
                    
                    x[0]+=1;
                    x[2]-=1;

                    
                }
                else if(rotation==2)
                {
                    y[0]+=1;
                    y[2]-=1;
                    
                    x[0]-=1;
                    x[2]+=1;
                    x[3]-=2;

                    
                }
                else if(rotation==3)
                {
                    y[0]-=1;
                    y[2]+=1;
                    y[3]-=2;
                    
                    x[0]-=1;
                    x[2]+=1;
                    
                }


                
                
            }
            
            
            
            if(rotation==3){
                    rotation=0;
                }
                else{
                    rotation++;
                }

        }

};


class BlockGameApp : public App
{
private:
  void drawScreen();

  void MoveToPlaced();
  void GameOver();

  Piece GetPiece(int type);
  int GetPieceColor(int type);

  int score = 0;
  int lines = 0;
  int level = 1;
  int placed[20][10];
  Piece current;
  int timePushDown = 500;
  u32_t oldTime =0;

  int encoderPos;



public:
  void init() override;
  void update() override;
};

#endif