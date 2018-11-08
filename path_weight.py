def printAllPathsUtil(self, u, d, visited, path,vertices):  
		visited[vertices.index(u)]= True
		path.append(u)
		if u==d: 
			return path
		else:
			for i in u.findall(".//"): 
				if visited[vertices.index(i)]==False: 
					path=self.printAllPathsUtil(i, d, visited, path,vertices)
					if(len(path)!=0):
						return path
		path.pop()
		visited[vertices.index(u)]= False
def printAllPaths(self,s,d,global_root):
	vertices=global_root.findall(".//")
	print(vertices)
	visited =[False]*(len(vertices))
	path = [] 
	path=self.printAllPathsUtil(s, d,visited, path,vertices)
	return path
def path_weight_nvgn(nodei,nodef,path_weight):
	#traverse down path from i to f
	path=printAllPaths(nodei,nodef)
	path_weight=0
	N=len(path)
	i=0
	prev=nodei
	while(i<N):
		if(self.nodei.doc!=self.path[i].doc):
			#they are in same doc
			path_weight=path_weight+link_weight_N(prev,path[i])
			i=i+1
			prev=path[i]
		else:
			#they are in diff doc
			#if it is retrieved element
			if path[i] in self.retrieved:
				path_weight=path_weight+link_weight_H(prev,path[i])
				i=i+1
				prev=path[i]
			else:
				i=i+1
	return path_weight
