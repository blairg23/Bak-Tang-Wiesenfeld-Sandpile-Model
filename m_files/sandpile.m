%Author: Blair Gemmer
%M 414 - Deterministic Models
%Cellular Automata: Bak-Tang-Wiesenfeld Sandpile Model
%Performs a CA plot of the BTW Sandpile Model. Can do an animation or just
%show the final plot, depending on user preferences. 
%
%User choices include:
%N = size of lattice
%critLevel = value of angle of repose
%dribble_point = can be middle or random
%lattice_state = can be static or random (all lattice points start at 0 or a random values)
%animation = True (1) or False (0), depending whether you want to see the animation
%

clear all, close all %clear variables

%View:
animation = 0;

%Initial state:
lattice_state = 'static';
dribble_point = 'middle';

%Lattice variables:
N = 100; %Size of lattice
critLevel = 4; %angle of repose

%Lattice:
if strcmp(lattice_state,'static')
    M = zeros(N, N);
else if strcmp(lattice_state,'random')
    M = randi([0, critLevel-1], N, N);
    end
end

iterations = 20000;
set(0, 'RecursionLimit', 1000); %resets maximum recursion limit
for i = 1:iterations
    if strcmp(dribble_point, 'random')
        x = randi(N);
        y = randi(N);
    else if strcmp(dribble_point, 'middle')
        x = N/2;
        y = N/2;
        end
    end
    
    M = feval('increase',M, x, y, critLevel, N); %performs function iterations

     if animation == 1
        imagesc(M); %shows matrix values as colors
        colormap('Copper'); %Hot, Jet, HSV, Autumn, Copper, and Gray are options here
        title({'Bak-Tang-Wiesenfeld Sandpile Model....';strcat('# of Iterations: ', num2str(i), '....')});
        mov(i) = getframe; %If you want to repeat the animation
     end
    %axis equal
    
end

%Final plot:
caxis([0 critLevel]); %shows axis for colorbar
imagesc(M); %shows matrix values as colors
colormap('Copper'); %Hot, Jet, HSV, Autumn, Copper, and Gray are options here
colorbar; %shows color bar
CmLength   = length(colormap); %how long colorbar should be
t = title({'Bak-Tang-Wiesenfeld Sandpile Model....';strcat('# of Iterations: ', num2str(i), '....')});
set(t, 'FontSize', 15);
set(gca, 'FontSize', 15);

%movie(mov, 30) %If you want to repeat the animation