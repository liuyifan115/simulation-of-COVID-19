# simulation-of-COVID-19

>## Given:
>1. population of Lanzhou = 300,0000
>2. cost_of_lock_down_population = 6000,0000 RMB/day
>3. infect_ratio = 3 || 10 || 50 / day
>4. infect_process = COVID-19_2.pdf
>5. shelter_size = 1000
>6. shelter_number = infected/shelter_size + 1
>7. shelter_time = 7 days/person
>8. cost_of_shelter = 200 RMB/(day, person)
>9. shelter collecting rate & ratio?? self estimated via 4.
>
>## Calculate day-by-day:
>1. infected_no, log IDs to file -> infected_day_x.txt
>2. cost_of_lock_down_population -> cost_of_lock_down_population_day_x.txt
>3. sheltered infor: ID with shelter_no -> shelter_day_x.txt
>4. total cost: -> total_cost.txt
>5. visualization (Excel, Python, ...) of the infection, sheleter, cost, day-by-day
>6. Pains & Gains via PPT

According to the given information, I make following assumptions:
1. totalPopulation = 3 000 000
2. lockdownCost = 200 RMB/(day*person)
3. infectRation = 3 **OR** 10 **OR** 50 (how many people one person can infect a day)
4. shelterSize = 1000
5. shelterTime = 7 
6. The person who has been infected will be sent to shelter after less than one day
7. simple mode **OR** complex mode
8. If the number of infected persons is more than one third of totalPopulation(1 000 000), the city will be locked down


