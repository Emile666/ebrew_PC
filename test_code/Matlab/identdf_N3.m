%-------------------------------------------------------------------
% $Id$
%
% $Log$
%
% System Identification function for a discrete second order model.
% 
% The transfer function of the model is the following:
%
%            Y(z^-1)       b1*z^-1 + b1*z^-2 + b1*z^-3
% Gs(z^-1) = ------- = ----------------------------------
%            U(z^-1)     1 + p1*z^-1 + p2*z^-2 + p3*z^-3
%
% This function is supplied with the input values u[k] and the 
% output values y[k] and calculates the values for b1, b2, b3, a1, a2 and a3.
%
% It uses the Recursive Least Square (RLS) method, combined with 
% adaptive directional forgetting.
%-------------------------------------------------------------------
% fi     - directional forgetting factor
% theta  - vector with the parameter estimates = [a1 a2 b1 b2]
% d      - regression vector = [ -y[k-1] -y[k-2] u[k-1] u[k-2] ]
% c      - Covariance Matrix
% lambda - lambda parameter
% eta    - parameter eta
% ep     - prediction error
% ksi    - auxiliary parameter ksi
% eps    - auxiliary parameter epsilon
% te     - parameter theta
% up     - previous controller output u[k-1]
% y      - current process output y[k]
% rho    - parameter rho
%-------------------------------------------------------------------
function [fi,theta,d,c,lambda,eta,ep,te,ksi,pp] = identdf_N3(fi,theta,d,c,lambda,eta,up,yk,rho)

% update of parameter estimates
ep    = yk - theta' * d % calc. prediction error (eq. 3.43)
ksi   = d' * c * d      % (eq. 3.42)
pp    = (c * d / (1 + ksi)) * ep; % eq. 3.41
theta = theta + pp

% update of system identification variables
if ksi > 0
   eps = fi - (1 - fi) / ksi % eq. 3.45
   c   = c - c * d * d' * c / (inv(eps) + ksi) % eq. 3.44

   lambda = fi * (lambda + ep * ep / (1 + ksi)) % eq. 3.48
   eta    = fi * (eta + 1)                      % eq. 3.47b
   te     = ep * ep / lambda                    % eq. 3.47a

   % oorspronkelijke formule uit het boek (3.47)
   fi     = 1 + (1 + rho) * (log(1 + ksi));      % eq. 3.47
   fi     = fi + ((eta + 1) * te / (1 + ksi + te) - 1) * ksi / (1 + ksi);
   fi     = 1 / fi

   %fi     = 1; % use this line for NO directional forgetting

   % This is the formula from sid.m
   fi     = 1 + ksi + ((eta + 1) * te / (1 + ksi + te) - 1) * ksi / (1 + ksi);
   fi     = 1 + (1 + rho) * log(fi);
   fi     = 1 / fi;
end

% Cyclic date substitution in regression vector
d(6) = d(5);
d(5) = d(4);
d(4) = up;
d(3) = d(2);
d(2) = d(1);
d(1) = -yk;
