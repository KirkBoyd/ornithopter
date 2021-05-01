%% Apr 24 Data Import
% Kirk Boyd 
% Apr 26, 2021
clc;clear;close all;set(0,'DefaultFigureWindowStyle','docked');set(0,'defaultTextInterpreter','latex');
delimiterIn = ' '; %all of the data points are separated by spaces in each line
headerlinesIn = 1; %each file has a header line with the names of the columns

%% Store all the names of the files
%files = dir(fullfile('reformat','*.txt'));
%filenames = {files.name};
%raw = importData(filenames);
%filenames {1,1} = Lc_calib_filename;
filenames {1} = 'apr24_calib_loadcell_reformat.txt'; %name of calibration curve file
filenames {2} = 'apr24_noWings1_loadcell_reformat.txt';  filenames{3} = 'apr24_noWings1_remote_reformat.txt';
filenames {4} = 'apr24_run1_loadcell_reformat.txt';      filenames {5} = 'apr24_run1_remote_reformat.txt';
filenames {6} = 'apr24_run2_loadcell_reformat.txt';      filenames {7} = 'apr24_run2_remote_reformat.txt';
filenames {8} = 'apr24_run3_loadcell_reformat.txt';      filenames {9} = 'apr24_run3_remote_reformat.txt';
filenames {10} = 'apr24_run4_loadcell_reformat.txt';     filenames {11} = 'apr24_run4_remote_reformat.txt';
filenames {12} = 'apr24_run5_loadcell_reformat.txt';     filenames {13} = 'apr24_run5_remote_reformat.txt';
filenames {14} = 'apr24_run6_loadcell_reformat.txt';     filenames {15} = 'apr24_run6_remote_reformat.txt';

%% Import data from all of the files
raw = cell(15,1);
for i = 1:length(filenames)
    raw{i} = importData(filenames{i});
    
end

%% Plot Load Cell Data  (Raw input)
subplot(8,1,1); plot(raw{1}.data); title('Calib'); ylabel("Load Cell Mass [g]");
subplot(8,1,2); plot(raw{2}.data); title('NoWings'); ylabel("Load Cell Mass [g]");
subplot(8,1,3); plot(raw{4}.data); title('Run1'); ylabel("Load Cell Mass [g]");
subplot(8,1,4); plot(raw{6}.data); title('Run2'); ylabel("Load Cell Mass [g]");
subplot(8,1,5); plot(raw{8}.data); title('Run3'); ylabel("Load Cell Mass [g]");
subplot(8,1,6); plot(raw{10}.data);title('Run4'); ylabel("Load Cell Mass [g]");
subplot(8,1,7); plot(raw{12}.data);title('Run5'); ylabel("Load Cell Mass [g]");
subplot(8,1,8); plot(raw{14}.data);title('Run6'); ylabel("Load Cell Mass [g]");
sgtitle("Load Cell Data Straight From Arduino");

%% Apply Mass offset
offset = cell(length(raw),1);
for i = 1:length(filenames)
    offset{i} = raw{i}.data - raw{i}.data(10);
end

%% Plot Load Cell Data  (With Mass Offset)
figure;
subplot(8,1,1); plot(offset{1}); title('Calib'); ylabel("Load Cell Mass [g]");
subplot(8,1,2); plot(offset{2}); title('NoWings'); ylabel("Load Cell Mass [g]");
subplot(8,1,3); plot(offset{4}); title('Run1'); ylabel("Load Cell Mass [g]");
subplot(8,1,4); plot(offset{6}); title('Run2'); ylabel("Load Cell Mass [g]");
subplot(8,1,5); plot(offset{8}); title('Run3'); ylabel("Load Cell Mass [g]");
subplot(8,1,6); plot(offset{10});title('Run4'); ylabel("Load Cell Mass [g]");
subplot(8,1,7); plot(offset{12});title('Run5'); ylabel("Load Cell Mass [g]");
subplot(8,1,8); plot(offset{14});title('Run6'); ylabel("Load Cell Mass [g]");
sgtitle("Load Cell Data with stationary mass subtracted");

%% Convert to Forces
force = cell(length(raw),1);
for i = 1:length(filenames)
    force{i} = (offset{i}*10^-3)*9.81;
end

%% Plot Load Cell Data  (With Mass Offset)
figure;
subplot(8,1,1); plot(force{1}); title('Calib'); ylabel("Force [N]");
subplot(8,1,2); plot(force{2}); title('NoWings'); ylabel("Force [N]");
subplot(8,1,3); plot(force{4}); title('Run1'); ylabel("Force [N]");
subplot(8,1,4); plot(force{6}); title('Run2'); ylabel("Force [N]");
subplot(8,1,5); plot(force{8}); title('Run3'); ylabel("Force [N]");
subplot(8,1,6); plot(force{10});title('Run4'); ylabel("Force [N]");
subplot(8,1,7); plot(force{12});title('Run5'); ylabel("Force [N]");
subplot(8,1,8); plot(force{14});title('Run6'); ylabel("Force [N]");
sgtitle("Force Measured by Load Cell (Mass * g)");

%% Plot a few flaps to compare to model
figure;
plot(force{14});title('Run6 just a few flaps'); ylabel("Force [N]");
xlim([250 400]);

%% Fix time scale

for i=3:2:15 
    raw{i}.data(:,1) = raw{i}.data(:,1) .* 10^-6; % move all values from miliseconds to seconds
    t_i = raw{i}.data(1,1);
    for j=1:length(raw{i}.data(:,1))
        raw{i}.data(j,1) = raw{i}.data(j,1)-t_i; % change them all to deltaT by subtracting the first time value from each
    end
end

%% Plot pwm values (not very useful)
figure;
subplot(7,1,1); plot(raw{3}.data(:,1),raw{3}.data(:,2)); title('NoWings');
subplot(7,1,2); plot(raw{5}.data(:,1),raw{5}.data(:,2)); title('Run1');
subplot(7,1,3); plot(raw{7}.data(:,1),raw{7}.data(:,2)); title('Run2'); 
subplot(7,1,4); plot(raw{9}.data(:,1),raw{9}.data(:,2)); title('Run3'); 
subplot(7,1,5); plot(raw{11}.data(:,1),raw{11}.data(:,2));title('Run4');
subplot(7,1,6); plot(raw{13}.data(:,1),raw{13}.data(:,2));title('Run5');
subplot(7,1,7); plot(raw{15}.data(:,1),raw{15}.data(:,2));title('Run6');
sgtitle('PWM signal written to ESC');

%% Remove all the crap from bad data
for i=3:2:15 
    for j=1:length(raw{i}.data(:,1))
        if raw{i}.data(j,3) > 1023 || raw{i}.data(j,3) <= 0 
            raw{i}.data(j,3) = NaN;
        end
        if raw{i}.data(j,4) > 1023 || raw{i}.data(j,4) <= 0
            raw{i}.data(j,4) = NaN;
        end
        if raw{i}.data(j,5) > 20 || raw{i}.data(j,4) == 0
            raw{i}.data(j,5) = NaN;
        end
        if raw{i}.data(j,6) > 20 || raw{i}.data(j,4) == 0
            raw{i}.data(j,6) = NaN;
        end
    end
end

%hallscaled = (13000,2,15);
for i=3:2:15
    for j=1:length(raw{i}.data(:,1))
        raw{i}.data(j,2) = raw{i}.data(j,2)/1023;
        raw{i}.data(j,3) = raw{i}.data(j,3)/1023; %scale the value down to be a percentage of the maximum analog signal that could be received
        raw{i}.data(j,4) = raw{i}.data(j,4)/1023; %same as above for hall 2
        raw{i}.data(j,2) = raw{i}.data(j,2)*10;
        raw{i}.data(j,3) = raw{i}.data(j,3)*10;
        raw{i}.data(j,4) = raw{i}.data(j,4)*10; %multiply by ten so they are a similar order of magnitude to acceleration
    end%for j
end%for i

for i=3:2:15
    for j=1:length(raw{i}.data(:,1))
        t(i,j,1) = raw{i}.data(j,1);
        esc(i,j,1) = raw{i}.data(j,2);
        hall(i,j,1) = raw{i}.data(j,3);
        hall(i,j,2) = raw{i}.data(j,4);
        accelx(i,j,1) = raw{i}.data(j,5);
        accely(i,j,1) = raw{i}.data(j,6);
    end
end

%% Plot Hall Effect Values
figure;
subplot(7,1,1); plot(raw{3}.data(:,1),  raw{3}.data(:,3),   raw{3}.data(:,1),   raw{3}.data(:,4));  title('NoWings');
subplot(7,1,2); plot(raw{5}.data(:,1),  raw{5}.data(:,3),   raw{5}.data(:,1),   raw{5}.data(:,4));  title('Run1');
subplot(7,1,3); plot(raw{7}.data(:,1),  raw{7}.data(:,3),   raw{7}.data(:,1),   raw{7}.data(:,4));  title('Run2');
subplot(7,1,4); plot(raw{9}.data(:,1),  raw{9}.data(:,3),   raw{9}.data(:,1),   raw{9}.data(:,4));  title('Run3');
subplot(7,1,5); plot(raw{11}.data(:,1), raw{11}.data(:,3),  raw{11}.data(:,1),  raw{11}.data(:,4)); title('Run4');
subplot(7,1,6); plot(raw{13}.data(:,1), raw{13}.data(:,3),  raw{13}.data(:,1),  raw{13}.data(:,4)); title('Run5');
subplot(7,1,7); plot(raw{15}.data(:,1), raw{15}.data(:,3),  raw{15}.data(:,1),  raw{15}.data(:,4)); title('Run6');
sgtitle("Hall Effect Sensors");

%% Plot Accelerometer Values
figure;
subplot(7,1,1); plot(raw{3}.data(:,1),  raw{3}.data(:,5),   raw{3}.data(:,1),   raw{3}.data(:,6));  title('NoWings');
subplot(7,1,2); plot(raw{5}.data(:,1),  raw{5}.data(:,5),   raw{5}.data(:,1),   raw{5}.data(:,6));  title('Run1');
subplot(7,1,3); plot(raw{7}.data(:,1),  raw{7}.data(:,5),   raw{7}.data(:,1),   raw{7}.data(:,6));  title('Run2');
subplot(7,1,4); plot(raw{9}.data(:,1),  raw{9}.data(:,5),   raw{9}.data(:,1),   raw{9}.data(:,6));  title('Run3');
subplot(7,1,5); plot(raw{11}.data(:,1), raw{11}.data(:,5),  raw{11}.data(:,1),  raw{11}.data(:,6)); title('Run4');
subplot(7,1,6); plot(raw{13}.data(:,1), raw{13}.data(:,5),  raw{13}.data(:,1),  raw{13}.data(:,6)); title('Run5');
subplot(7,1,7); plot(raw{15}.data(:,1), raw{15}.data(:,5),  raw{15}.data(:,1),  raw{15}.data(:,6)); title('Run6');
sgtitle('Acceleration')

%% Put em all in one graph just to see
figure;
subplot(7,1,1); 
plot(t(3,:),  esc(3,:),  'LineWidth',2);
hold on;
plot(t(3,:),  hall(3,:,1),   t(3,:),   hall(3,:,2),   t(3,:),   accelx(3,:),   t(3,:),   accely(3,:));  title('NoWings');
subplot(7,1,2); plot(raw{5}.data(:,1),  raw{5}.data(:,2),   raw{5}.data(:,1),  raw{5}.data(:,3),   raw{5}.data(:,1),   raw{5}.data(:,4),   raw{5}.data(:,1),   raw{5}.data(:,5),   raw{5}.data(:,1),   raw{5}.data(:,6));  title('Run1');
subplot(7,1,3); plot(raw{7}.data(:,1),  raw{7}.data(:,2),   raw{7}.data(:,1),  raw{7}.data(:,3),   raw{7}.data(:,1),   raw{7}.data(:,4),   raw{7}.data(:,1),   raw{7}.data(:,5),   raw{7}.data(:,1),   raw{7}.data(:,6));  title('Run2');
subplot(7,1,4); plot(raw{9}.data(:,1),  raw{9}.data(:,2),   raw{9}.data(:,1),  raw{9}.data(:,3),   raw{9}.data(:,1),   raw{9}.data(:,4),   raw{9}.data(:,1),   raw{9}.data(:,5),   raw{9}.data(:,1),   raw{9}.data(:,6));  title('Run3');
subplot(7,1,5); plot(raw{11}.data(:,1), raw{11}.data(:,2),  raw{11}.data(:,1), raw{11}.data(:,3),  raw{11}.data(:,1),  raw{11}.data(:,4),  raw{11}.data(:,1),  raw{11}.data(:,5),  raw{11}.data(:,1),  raw{11}.data(:,6)); title('Run4');
subplot(7,1,6); plot(raw{13}.data(:,1), raw{13}.data(:,2),  raw{13}.data(:,1), raw{13}.data(:,3),  raw{13}.data(:,1),  raw{13}.data(:,4),  raw{13}.data(:,1),  raw{13}.data(:,5),  raw{13}.data(:,1),  raw{13}.data(:,6)); title('Run5');
subplot(7,1,7); plot(raw{15}.data(:,1), raw{15}.data(:,2),  raw{15}.data(:,1), raw{15}.data(:,3),  raw{15}.data(:,1),  raw{15}.data(:,4),  raw{15}.data(:,1),  raw{15}.data(:,5),  raw{15}.data(:,1),  raw{15}.data(:,6)); title('Run6');
sgtitle('Combo')

figure;
plot(raw{15}.data(:,1), raw{15}.data(:,2),  raw{15}.data(:,1), raw{15}.data(:,3),  raw{15}.data(:,1),  raw{15}.data(:,4),  raw{15}.data(:,1),  raw{15}.data(:,5),  raw{15}.data(:,1),  raw{15}.data(:,6)); title('Run6');

figure;
plot(raw{15}.data(:,1), raw{15}.data(:,2),  raw{15}.data(:,1), raw{15}.data(:,3),  raw{15}.data(:,1),  raw{15}.data(:,4),  raw{15}.data(:,1),  raw{15}.data(:,5),  raw{15}.data(:,1),  raw{15}.data(:,6)); title('Run6');
xlim([105 110]);
% 
% combined = zeros(13000,15,8);
% 
% combined(:,3,1) = raw{3}.data(:,1);