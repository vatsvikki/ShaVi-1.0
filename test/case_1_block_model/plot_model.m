clear all;
clc;
tiledlayout(2,1);
nexttile;
real = load('real_model_block.txt');
pcolor(real); axis ij; shading flat; colorbar; 
xlabel('Length (m)');
ylabel('Depth (m)');
yticks([10 20 30 40 50]);
yticklabels({'100','200','300','400','500'});
xticks([10 20 30 40 50 60 70 80 90 100]);
xticklabels({'100','200','300','400','500','600', '700', '800', '900', '1000'})
title('True Model');
c = colorbar;
c.Label.String = 'Velocity (m/s)';


nexttile;
initial = load('initial_model_block.txt');
pcolor(initial); axis ij; shading flat; colorbar; clim([1800 2000]);
xlabel('Length (m)');
ylabel('Depth (m)');
yticks([10 20 30 40 50]);
yticklabels({'100','200','300','400','500'});
xticks([10 20 30 40 50 60 70 80 90 100]);
xticklabels({'100','200','300','400','500','600', '700', '800', '900', '1000'})
title('Initial Model');
c = colorbar;
c.Label.String = 'Velocity (m/s)';

