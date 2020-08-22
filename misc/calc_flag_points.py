#!/usr/bin/python

basic_costs_stolen_flag_in_points = 10
services = [
    { "stolen_flags": 100, "defended_flags": 9 }, # service0 
    { "stolen_flags": 0, "defended_flags": 0 }, # service1
    { "stolen_flags": 12, "defended_flags": 50 }, # service2
    { "stolen_flags": 1, "defended_flags": 0 }, # service3
    { "stolen_flags": 0, "defended_flags": 10 } # service4
]
teams = 10 # number of teams
all_sf = 0 # number of all flags which was stolen
all_df = 0 # number of all flags which was defended
for s in services:
    all_sf = all_sf + s["stolen_flags"]
    all_df = all_df + s["defended_flags"]
print("all_sf=" + str(all_sf) + ", all_df=" + str(all_df))

sp4d = len(services) * basic_costs_stolen_flag_in_points
dp4d = len(services) * (teams - 1) * basic_costs_stolen_flag_in_points
print("sp4d=%d, dp4d=%d" % (sp4d, dp4d))

sf_rsum = 0 
df_rsum = 0

# calcululate reverse proportional df_rsum and sf_rsum
for s in services:
    if s["stolen_flags"] > 0:
        s["rpsf"] = float(all_sf / s["stolen_flags"])
    else:
        s["rpsf"] = float(all_sf / 1.0)

    if s["defended_flags"] > 0:
        s["rpdf"] = float(all_df / s["defended_flags"])
    else:
        s["rpdf"] = float(all_df / 1.0)

    sf_rsum = sf_rsum + s["rpsf"]
    df_rsum = df_rsum + s["rpdf"]

# calculate flag costs
i = 0
for s in services:
    cost_attack_flag = 0
    if sf_rsum == 0:
        cost_attack_flag = sp4d
    else:
        cost_attack_flag = sp4d * (s["rpsf"] / sf_rsum)
        
    cost_defence_flag = 0
    if df_rsum == 0:
        cost_defence_flag = dp4d
    else:
        cost_defence_flag = dp4d * (s["rpdf"] / df_rsum)
    print("service%d: cost_attack_flag=%.2f, cost_defence_flag=%.2f" % (i, cost_attack_flag, cost_defence_flag))
    i = i + 1