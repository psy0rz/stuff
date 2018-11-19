//(C) edwin@datux.nl GPL

//proportional fanspeed controller
//hoe vochtiger de douche/zolder is ten opzichte van de overloop, hoe sneller de fan gezet word.
//dus tijdens het douchen maximale fan, en daarna zal hij een tijd op lagere snelheid naventileren. 
//(doordat de water uit de douchebak/handoeken verdampt)

//als fan gebruik ik een Zehnder Stork Comfofan S, met een ESP8266 er op met daarop ESPEasy geinstalleerd.
//(de fan werkt perfect met PWM aansturing op de 0-10v ingang)


let tokens = args[0].split(' ');
log("Zolder fan script: ",tokens);

//vocht percentages
overloop=tokens[1];
douche=tokens[2];
slaapkamer=tokens[0];

//minimum/max snelheid fan
min=5;
max=100;

//maximaal haalbare vochtigheid douche
max_vochtig=96;

//range waarin douche kan varieren (overloop is laagst haalbare, max_vochtig het hoogst haalbare)
range=96-overloop;

power_slaapkamer=(slaapkamer-overloop)/range*100;
log("Gewenste power slaapkamer", power_slaapkamer);

power_douche=(douche-overloop)/range*100;
log("Gewenste power douche", power_douche);

//kies power
power=Math.max(min, power_douche, power_slaapkamer);
power=Math.min(max, power);
log("Power", power);

//bereken pwm waarde fan controller
pwm=1024-(power/100*1024);
await fetch("http://192.168.13.56/control?cmd=PWM,4,"+pwm);
log("Set controller pwm to ",pwm);


