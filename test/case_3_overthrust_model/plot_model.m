clear all;
clc;
tiledlayout(2,1);
nexttile;
real = load('real_overthrust_model.txt');
pcolor(real); axis ij; shading flat; colorbar; clim([1500 5500]);
xlabel('Length (m)');
ylabel('Depth (m)');
yticks([50 100 150]);
yticklabels({'500','1000','1500'});
xticks([50 100 150 200 250 300 350 400 450 500]);
xticklabels({'500','1000','1500','2000','2500','3000', '3500', '4000', '4500', '5000'})
title('True Model');
c = colorbar;
c.Label.String = 'Velocity (m/s)';


nexttile;
initial = load('initial_overthrust_model.txt');
pcolor(initial); axis ij; shading flat; colorbar; clim([1500 5500]);
xlabel('Length (m)');
ylabel('Depth (m)');
yticks([50 100 150]);
yticklabels({'500','1000','1500'});
xticks([50 100 150 200 250 300 350 400 450 500]);
xticklabels({'500','1000','1500','2000','2500','3000', '3500', '4000', '4500', '5000'})
title('Initial Model');
c = colorbar;
c.Label.String = 'Velocity (m/s)';

