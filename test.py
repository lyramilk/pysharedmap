import sharedmap
import datetime




def t1():
	wmap = sharedmap.rbtree();
	wmap.set("aaa","323");
	wmap.set("zb","1233");
	wmap.set("tes","32z");
	wmap.set("红","32是");
	wmap.set("红",str(datetime.datetime.now()));

	wmap.share("songkey2rid2_all_base_on_blacklist",force=True);


print("测试写入");
t1();


def t2():
	rmap = sharedmap.sharedmap("songkey2rid2_all_base_on_blacklist");
	print(rmap.get("红"),type(rmap.get("红")));

print("测试读取");
t2();



sbm = sharedmap.sharedbitmap.create("bbt",20000000,force=True);
sbm.set(20000000,True);

print(sbm.get(15475645));
print(sbm.get(20000000));
