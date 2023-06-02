%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Analisi filtro del secondo ordine per inverter con controllo a PWM
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
clear all;
close all;
%
 %%syms k0 kp ;
%% Guadagni ipotetici dei trasduttori 
%
kSv = .033;        % fattore di trasduzione della tensione:
 
%% Parametri fisici dell'inverter
%
fsw = 10;         % frequenza di switching di  Introduzione Esercitazione n.2.pdf
Vdc = 450;          % tensione dc nominale
Rth = 5;         % °C/W
Cth = 1.2;          % J/°C
fc=0.4% frequenza di banda passante 

%
%% Modello dinamico dell'inverter
%
s = tf('s');
D = 1 + s*(Rth*Cth);
tau=Rth*Cth;
G2 = Rth/D;%G2 è il T(s)  sistema da regolare 
%
bode(G2);

figure;
bode(G2);
%
figure;
step(G2);
%
%% Progetto di un regolatore PID
%
% Calcolo il guadagno dei trasduttori lungo l'anello
%
Gadc = 1/3.3;                   % guadagno equivalente dell'ADC
Gpwm = Vdc;                % guadagno equivalente del modulatore ,, 
%Vdc/1440 , dive 1440 è i max che i timer conta  per avere i
%periodo di pwm  
G0 = kSv*Gpwm*Gadc;             % guadagno totale incluso sensore di tensione
%
%
wcr = 2*pi*fc;               % frequenza di taglio desiderata
[MG2, PhiG2] = bode(G2,wcr);     % modulo e fase di G2 alla pulsazione wcr
%

beta = 0.9;
wr = 1/tau;% di T(s)
wz = beta*wr;
k0 = wcr/(G0*MG2*(1+(wcr/(beta*wr))^2)); % valore di k0 per avere wcr
%
% Criterio di progetto: posiziono i due zeri del regolatore alla stessa
% pulsazione pari a beta*wr

R = (k0/s)*(1+s/wz)^2;                    % regolatore
T = R*G2*G0;    % catena aperta                          % guadagno d'anello
W = T/(1+T);                             % FdT a catena chiusa
%
figure;
bode(R,T,W);
grid on;
%
figure;
step(W);
%
%% Coefficienti del regolatore PID (tempo continuo)
%
ki = k0;
kp = 2*ki/(beta*wr);
kd = ki/(beta*wr)^2;
%
%% Mappatura dei coefficienti per il controllore digitale
%
kpd = round(kp);
kid = round(ki/fsw);
kdd = round(kd*fsw);