%-------------------------------------------------------------------
% $Id$
%
% $Log$
%
% Pagina 39, Listing 3.3: Recursive Least Squares, but now with
%                         adaptive directional forgetting
%-------------------------------------------------------------------

% Input variables, controller output
%      1      2       3       4       5      6       7       8       9
u = [ -0.0740 1.3174  1.6804  1.1934 -0.4183 0.8779  1.6688 -1.1308 -0.8360 -0.4164  0.2271 -1.4945 0.6550 -1.3920 -0.9298  0.6184 0.7664 -1.0097  1.3486 -0.7796  1.0197 -1.4165 1.6342 ];
 
% Output variables, process output
%      1      2       3       4      5       6       7       8       9
y = [ -0.0949 0.0322 -0.0551 -0.0391 0.1397  0.4680  0.4351  0.3682  0.7136  0.7245  0.5573 0.3392 0.3994  0.0751  0.1649 0.0524 -0.2315 -0.3277 -0.1159 -0.3156 -0.0789 -0.1953 0.0827 ];

N = 23; % number of identification steps

% Initialization of Recursive Identification (listing 3.2)
d      = [-y(2); -y(1); u(2); u(1)]; % regression vector
theta  = [0.1; 0.2; 0.1; 0.2];
c      = 1e+9 * eye(4);              % Covariance matrix
rho    = 0.99; 
phi    = 1; 
lambda = 0.001;
eta    = 0.000001;

for k=3:N
   [phi,theta,d,c,lambda,eta,ep,te,ksi,pp]=identdf_N2(phi,theta,d,c,lambda,eta,u(k),y(k),rho);
   a1(k)=theta(1);
   a2(k)=theta(2);
   b1(k)=theta(3);
   b2(k)=theta(4);
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