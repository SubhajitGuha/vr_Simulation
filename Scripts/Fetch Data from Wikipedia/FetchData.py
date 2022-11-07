import os
import wikipedi


os.system("cls")

item=input("What are you looking for : ")
result=wikipedia.search(item)
page=wikipedia.page(result[0])

summary=page.summary
f=open(item+".txt","w+")
f.write(summary)
f.close()

print(summary)
