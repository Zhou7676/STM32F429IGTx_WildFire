import json

p_d = {'Tom': 29, 'Jack': 20, "Jim": 12}
# print(p_d)

p_to_j = json.dumps(p_d)
print(p_to_j)

j_to_p = json.loads(p_to_j)
# print(j_to_p)