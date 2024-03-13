import os

print(os.getcwd())

gameobjects: list[str] = ["sound", "sprite", "text"]
ffi_index_members: dict[str, list[str]] = dict()
ffi_newindex_members: dict[str, list[str]] = dict()

for objtext in gameobjects:
    functions: list[str] = []
    objfp = open(f"code/gameobjects/include/{objtext}.h")
    for line in objfp.readlines():
        if "get" in line:
            # this is a index_member
            type_name = line.split('chrus_')[1].split('_get')[0]
            member_name = line.split('(')[0].split('get_')[1]
            if ffi_index_members.get(type_name):
                ffi_index_members[type_name].append(member_name)
            else:
                ffi_index_members[type_name] = [member_name]
        if "set" in line:
            # this is a newindex_member
            type_name = line.split('chrus_')[1].split('_set')[0]
            member_name = line.split('(')[0].split('get_')[1]
            if ffi_newindex_members.get(type_name):
                ffi_newindex_members[type_name].append(member_name)
            else:
                ffi_newindex_members[type_name] = [member_name]

for key, indexmemlist in ffi_index_members.items():
    # print(key, indexmemlist)
    final_string = f"local {key}_members = {'{'}\n"
    for member in indexmemlist:
        final_string += f"    {member} = function(node)\n        return lchrus.chrus_{key}_get_{member}(node)\n    end,\n"
    final_string += "}\n"
    print(final_string)