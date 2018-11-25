if(onoff==0){ // if sleep mode
  		waitForPb(); // wait for turn on signal
  		onoff=1;//if pb pressed, turn on sensor mode
  	}else{
  		if(sensorType==0){
  			//initialize sensor0
  			//run thread
  			waitForPb(); // wait for turn on signal
  			//turn off sensor0
  		}else if(sensorType==1){
  			//initialize sensor1
  			//run thread
  			waitForPb(); // wait for turn on signal
  			//turn off sensor1
  		}else if(sensorType==2){
  			//initialize sensor2
  			//run thread
  			waitForPb(); // wait for turn on signal
  			//turn off sensor2
  		}else if(sensorType==3){
  			//initialize sensor3
  			//run thread
  			waitForPb(); // wait for turn on signal
  			//turn off sensor3
  		}
  		
  		pbtime=0;//save current time;
  		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)!=1){ //while pb is pressed
  			if(timerFlag==1){
  				timerFlag=0;
  				pbtime++;
  			}
  			if(pbtime>=15){
  				onoff=0;
  			}

  		}
  		if(onoff==1){
  			sensorType=(sensorType+1)%5; // rotate sensor type
  		}





  	}
 