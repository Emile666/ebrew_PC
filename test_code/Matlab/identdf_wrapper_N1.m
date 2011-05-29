%-------------------------------------------------------------------
% $Id$
%
% $Log$
%
% Pagina 39, Listing 3.3: Recursive Least Squares, but now with
%                         adaptive directional forgetting
%-------------------------------------------------------------------

% Input variables, controller output
u = [-0.6918 0.858 1.254 -1.5937 -1.441 0.5711 -0.3999 0.69]
% Output variables, process output
y = [0.0071 -0.1252 0.0583 0.3070 -0.0744 -0.342 -0.1839 -0.2539]
N = 8; % number of identification steps

% Initialization of Recursive Identification (listing 3.2)
d      = [-y(1); u(1)];        % regression vector
theta  = [0.1; 0.1];
c      = 1e9 * eye(2);        % Covariance matrix
rho    = 0.99; 
phi    = 1; 
lambda = 0.001;
eta    = 0.000001;

for k=2:N
   [phi,theta,d,c,lambda,eta,ep,te,ksi,pp]=identdf_N1(phi,theta,d,c,lambda,eta,u(k),y(k),rho);
   a1(k)=theta(1)
   b1(k)=theta(2)
end;

figure;
plot(a1,'k');
hold on;
plot(b1,'b');
plot(ep,'r');
xlabel('time steps')
ylabel('theta, e')