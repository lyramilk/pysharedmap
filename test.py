import sharedmap

def t1():
	wmap = sharedmap.rbtree();
	wmap.set("aaa","323");
	wmap.set("zb","1233");
	wmap.set("tes","32z");
	wmap.set("红","32是");
	print(wmap.get("tes").decode("utf8"));
	wmap.share("test:3325",force=True);


print("测试写入");
t1();



def t2():
	rmap = sharedmap.sharedmap("test:3325");
	print(rmap.get("红").decode("utf8"));

print("测试读取");
t2();