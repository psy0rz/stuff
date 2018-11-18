
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


