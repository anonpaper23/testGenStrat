Array=readtable('..\..\Evaluation Results\RQ2\#RQ2NumCasesThatAreSignificantRevised.xlsx');
SAAccuracy = Array.ens;
RSAccuracy = Array.rs;
LRAccuracy = Array.logreg;
RTAccuracy = Array.regtree;
grouping1 = {Array.XAI,Array.ensStatus};
grouping2 = {Array.XAI,Array.logregStatus};
grouping3 = {Array.XAI,Array.regtreeStatus};


h = tight_subplot(1, 3, [0.1 0.05], [0.1 0.05], [0.1 0.05]);
axes(h(1))
x = linspace(0,1);
y = linspace(0,1);
stem(y);
plot(x,y,':');
hold on;
gscatter(RSAccuracy,SAAccuracy,grouping1,'BRBR','ooxxx');
legend('Location','northeastoutside');
xlabel('RS')
ylabel('SA')
xlim([0 1])
ylim([0.5 1])
legend('Location','best');
%%%%%%%%%%%
axes(h(2));
subplot(1,3,2);
x = linspace(0,1);
y = linspace(0,1);
stem(y);
plot(x,y,':');
hold on;
gscatter(RSAccuracy,LRAccuracy,grouping2,'RBRBM','ooxxx');
legend('Location','northeastoutside');
xlabel('RS')
ylabel('LR')
xlim([0 1])
ylim([0.5 1])
legend('Location','best');
%%%%%%%%%
axes(h(3));
subplot(1,3,3);
x = linspace(0,1);
y = linspace(0,1);
stem(y);
plot(x,y,':');
hold on;
gscatter(RSAccuracy,RTAccuracy,grouping3,'RBMRB','ooxxx');
legend('Location','best');
xlabel('RS')
ylabel('RT')
xlim([0 1])
ylim([0.5 1])

%https://www.mathworks.com/matlabcentral/answers/292197-color-different-scatter-points-based-on-group