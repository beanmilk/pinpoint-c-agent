from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time

driver = webdriver.Chrome()
driver.get("http://10.34.130.134:8001/")
elem = driver.find_element_by_name("type")
elem.send_keys("pycon")
elem.send_keys(Keys.ENTER)
print(driver.page_source)
driver.close()