# simulation-of-COVID-19

## Given:
1. population of Lanzhou = 300,0000
2. cost_of_lock_down_population = 6000,0000 RMB/day
3. infect_ratio = 3 || 10 || 50 / day
4. infect_process = COVID-19_2.pdf
5. shelter_size = 1000
6. shelter_number = infected/shelter_size + 1
7. shelter_time = 7 days/person
8. cost_of_shelter = 200 RMB/(day, person)
9. shelter collecting rate & ratio?? self estimated via 4.

## Calculate day-by-day:
1. infected_no, log IDs to file -> infected_day_x.txt
2. cost_of_lock_down_population -> cost_of_lock_down_population_day_x.txt
3. sheltered infor: ID with shelter_no -> shelter_day_x.txt
4. total cost: -> total_cost.txt
5. visualization (Excel, Python, ...) of the infection, sheleter, cost, day-by-day
6. Pains & Gains via PPT