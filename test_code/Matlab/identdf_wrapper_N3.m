%-------------------------------------------------------------------
% $Id$
%
% $Log$
%
% Pagina 39, Listing 3.3: Recursive Least Squares, but now with
%                         adaptive directional forgetting
% Page 126, Gd(z), example 4.5
%  b1 =  0.0706 b2 = 0.1416 b3 = 0.0136
%  a1 = -1.2747 a2 = 0.5361 a3 = 0.0302
%-------------------------------------------------------------------

% Input variables, controller output
%      1      2       3       4       5      6       7       8       9
u = [-0.5638 -0.2284 -0.8515 -0.5830 0.2703 0.3731 -0.4709 -0.1284 0.2340 -0.4086 0.3099 -0.5626 0.6497 0.7958 -0.6926 -0.0790 0.0359 0.0253 -0.5276 0.1018 -0.5520 0.2082 0.7909 0.1212 0.0163 0.1848 0.3978 -0.4665 0.0770 -0.7014 ];

% Output variables, process output
%      1      2       3       4      5       6       7       8       9
y = [0.0000 -0.0398 -0.1467 -0.2658 -0.4238 -0.4683 -0.3051 -0.1018 -0.0227 0.0268 0.0519 0.0197 -0.0049 -0.0480 0.0814 0.2023 0.1228 0.0276 -0.0310 -0.0912 -0.1676 -0.1956 -0.2188 -0.0911 0.1304 0.2507 0.2695 0.2596 0.2048 0.0585 ];

N = 23; % number of identification steps

% Initialization of Recursive Identification (listing 3.2)
d      = [-y(3); -y(2); -y(1); u(3); u(2); u(1)]; % regression vector
theta  = [0.1; 0.2; 0.3; 0.1; 0.2; 0.3];
c      = 1.0e+09 * eye(6);              % Covariance matrix
rho    = 0.99; 
phi    = 1; 
lambda = 0.001;
eta    = 0.000001;

for k=4:N
   [phi,theta,d,c,lambda,eta,ep,te,ksi,pp]=identdf_N3(phi,theta,d,c,lambda,eta,u(k),y(k),rho);
   a1(k)=theta(1);
   a2(k)=theta(2);
   a3(k)=theta(3);
   b1(k)=theta(4);
   b2(k)=theta(5);
   b3(k)=theta(6);
end;

figure;
plot(a1,'k');
hold on;
plot(a2,'c');
plot(b1,'b');
plot(b2,'g');
plot(ep,'r');
xlabel('time steps')
ylabel('theta, e')