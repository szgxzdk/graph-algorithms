clear  
clc  
load('12x4.txt', 'data')
X = X12x4(:, 1:(size(X12x4, 2) - 1));
%X = [ones(12, 1), X];
Y = X12x4(:, size(X12x4, 2));
  
%B=(X'* X)\(X'* Y);
%B = regress(Y, X);

% A = X' * X;
% ce = poly(eig(A));
% cesize = max(size(ce));
% p = [0 ce(1:(cesize-1))];
% s = (-1)^(max(size(A))+1);
% B = s*polyvalm(p,A);
% B

A = X' * X;
C = A;
for i = 1 : size(A, 1)
   for j = 1 : size(A, 2)
      tmp = A;
      tmp(i, :) = [];
      tmp(:, j) = [];
      C(i, j) = (-1)^(i+j) * det(tmp);
      if i == 1 && j == 1
         tmp ;
      end
   end
end
C = C';
C;


X = [5507293.750000 2576273.000000 1465510.375000 
2576273.000000 1240116.500000 668078.250000 
1465510.375000 668078.250000 398716.062500 ];
% X = [1 17 61
%      6 89 73
%      19 13 7];
t = ones(6, 1);
result = X(1, 1) * X(2, 2) * X(3, 3) + X(1, 2) * X(2, 3) * X(3, 1) + X(1, 3) * X(2, 1) * X(3, 2) - X(1, 1) * X(2, 3) * X(3, 2) - X(1, 2) * X(2, 1) * X(3, 3) - X(1, 3) * X(2, 2) * X(3, 1);
t(1, 1) = X(1, 1) * X(2, 2) * X(3, 3);
t(2, 1) = X(1, 2) * X(2, 3) * X(3, 1);
t(3, 1) = X(1, 3) * X(2, 1) * X(3, 2);
t(4, 1) = X(1, 1) * X(2, 3) * X(3, 2);
t(5, 1) = X(1, 2) * X(2, 1) * X(3, 3);
t(6, 1) = X(1, 3) * X(2, 2) * X(3, 1);

tmp = 0;
for i = 1:3
   tmp = tmp + t(i);
   fprintf('+ %f = %f\n', t(i), tmp);
end
for i = 4:6
   tmp = tmp - t(i);
   fprintf('- %f = %f\n', t(i), tmp);
end
det(X)
