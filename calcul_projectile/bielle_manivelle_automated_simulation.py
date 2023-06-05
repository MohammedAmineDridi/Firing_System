from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By

from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

import time
import sys



options = webdriver.ChromeOptions()
options.add_argument("--user-data-dir=D:/automated_robin_hood/python_selenium_phet")
options.add_experimental_option("detach", True)
driver = webdriver.Chrome(options=options) # ,executable_path='D:/chrome_web_driver/chromedriver.exe'


# Provide the correct executable path
#driver = webdriver.Chrome()
#params 

r = sys.argv[1] # raduis of manivelle 
l = sys.argv[2]  # translation position of the piston 
n = sys.argv[3]  # rotation velocity of the manivelle 


wait = WebDriverWait(driver, 10)

# Navigate to simulator homepage
driver.get("http://www.pats.ch/formulaire/cinematique/cinematique3.aspx")

time.sleep(3) # delay for 3 second 

# step 1 : insert (r : raduis of manivelle) 

initial_heigth_bar = driver.find_element(By.ID, "ctl00_ContenuPrincipal_TextBoxr") # r: raduis (mm)
initial_heigth_bar.clear()
initial_heigth_bar.send_keys(str(r))

time.sleep(1) # delay for 1 second .

#step 2 : insert (l : position of piston)

initial_speed = driver.find_element(By.ID, "ctl00_ContenuPrincipal_TextBoxl") # l : position (mm)
initial_speed.clear()
initial_speed.send_keys(str(l))

time.sleep(1) # delay for 1 second .

#step 3 : insert (n : rotation velocity)

initial_speed = driver.find_element(By.ID, "ctl00_ContenuPrincipal_TextBoxn") # n : rotation velocity 
initial_speed.clear()
initial_speed.send_keys(str(n))

time.sleep(1) # delay for 1 second 

# step 3 : click btn 'lancer le calcul' to start the calculation .

slow_motion_cb = driver.find_element(By.ID, "ctl00_ContenuPrincipal_Button1") # start calculation btn
slow_motion_cb.click()
time.sleep(1) # delay for 1 second .

#step 4 : just verify your angle of manivelle rotation ....

# Close the browser
# driver.quit()