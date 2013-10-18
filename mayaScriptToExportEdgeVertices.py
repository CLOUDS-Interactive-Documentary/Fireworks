# mt.py
# 
# 
if 'cmds' not in locals():
	import maya.cmds as cmds
import math

edges = cmds.ls(sl=1,fl=1)

cmds.ConvertSelectionToVertices()
vertices = cmds.ls(sl=1, fl=1)

outVerts = []

for e in edges:

    ev = cmds.polyInfo( e, ev=True )[0].split()
    for i in ev:#[2:]:
    	outVerts.append( vertices[ int(i) ] ) 
        
print outVerts
cmds.select( edges, r=1 )