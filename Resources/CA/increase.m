function[m] = increase(m, x, y, critLevel, N)
    %Function to increase the slope of a given part of the NxN lattice, m
    %at point (x, y). If it reaches a given critical level, the slope
    %topples back to 0.
    isBoundary = 0;
    %If we reached a boundary:
    if (x <= 1) || (x >= N)
        isBoundary = 1;        
    else if (y <= 1) || (y >= N)
        isBoundary = 1;
        end
    end
    
    if isBoundary == 0
        m(x, y) = m(x, y) + 1;        
    end
    
    if m(x, y) >= critLevel %If we're past the critical slope
        %Start the cascade:
        m(x, y) = m(x, y) - 4;         
        m = feval('increase', m, x+1, y, critLevel, N);
        m = feval('increase', m, x-1, y, critLevel, N);
        m = feval('increase', m, x, y+1, critLevel, N);
        m = feval('increase', m, x, y-1, critLevel, N);
    end
end