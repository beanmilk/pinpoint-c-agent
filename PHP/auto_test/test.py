import re

'''
<html><head></head><body>app\AppDate::outputDate start <br> date start <br> string(10) "2019/08/10"
date end <br>2019/08/10<br>10.35.240.28<br>10.34.130.134:8001<br><br><br><br>NULL
app\AppDate::outputDate end <br></body></html>
'''

# print(re.match('www.xx.com','www.xx.com'))

pattern = re.compile("o[gh]")
pattern.fullmatch("dog")      # No match as "o" is not at the start of "dog".
pattern.fullmatch("ogre")     # No match as not the full string matches.
pattern.fullmatch("doggie", 1, 3)