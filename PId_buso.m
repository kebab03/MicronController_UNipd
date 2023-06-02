%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Analisi filtro del secondo ordine per inverter con controllo a PWM
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
clear all;
close all;
%
%% Guadagni ipotetici dei trasduttori 
%
kSv = .03/540;        % fattore di trasduzione della tensione:
%                       1V/160V
%
Rsh = 3.3/50;        % sensore di corrente (shunt): 
%                       fattore di scala della corrente 1V/11A
%
%% Parametri fisici dell'inverter
%
fsw = 50e3;         % frequenza di switching 
Vdc = 450;          % tensione dc nominale
L = 180e-6;         % induttanza di uscita
C = 10e-6;          % capacit� di uscita
Rload = 12;         % resistenza di carico
%
%% Ripple picco-picco di corrente
%
DeltaIpk = Vdc/(8*fsw*L);
%
%% Modello dinamico dell'inverter
%
s = tf('s');
D = 1 + s*(L/Rload) + L*C*s^2;
G1 = (Vdc/Rload)*(1+s*Rload*C)/D;
G2 = 1/D;
%
bode(G1,Vdc/(s*L));
%
G21 = 1/(1 + s*(L/(Rload*0.8)) + L*C*s^2);
G22 = 1/(1 + s*(L/(Rload*1.2)) + L*C*s^2);
G23 = 1/(1 + s*(L/(Rload*2)) + L*C*s^2);
%
figure;
bode(G21,G2,G22,G23);
%
figure;
step(G2);
%
%% Progetto di un regolatore PID
%
% Calcolo il guadagno dei trasduttori lungo l'anello
%
Gadc = 1/3.3;                   % guadagno equivalente dell'ADC
Gpwm = Vdc/1440;                % guadagno equivalente del modulatore
G0 = kSv*Gpwm*Gadc;             % guadagno totale incluso sensore di tensione
%
%
wcr = 2*pi*fsw/10;               % frequenza di taglio desiderata
[MG2, PhiG2] = bode(G2,wcr);     % modulo e fase di G2 alla pulsazione wcr
%
% Criterio di progetto: posiziono i due zeri del regolatore alla stessa
% pulsazione pari a beta*wr
%
beta = 0.9;
wr = 1/sqrt(L*C);
wz = beta*wr;
k0 = wcr/(G0*MG2*(1+(wcr/(beta*wr))^2)); % valore di k0 per avere wcr
%
R = (k0/s)*(1+s/wz)^2;                   % regolatore
T = R*G2*G0;                             % guadagno d'anello
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