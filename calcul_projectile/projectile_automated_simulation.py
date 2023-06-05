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

init_height = sys.argv[1]
vi = sys.argv[2]
angle = sys.argv[3]

wait = WebDriverWait(driver, 10)

# Navigate to simulator homepage
driver.get("https://www.walter-fendt.de/html5/phen/projectile_en.htm")

time.sleep(3) # delay for 3 second

# step 1 : click reset btn

reset_button = wait.until(EC.element_to_be_clickable((By.ID, "bu1")))
driver.execute_script("arguments[0].scrollIntoView();", reset_button)
reset_button.click()

time.sleep(3) # delay for 3 seconds .

# step 2 : params settings 
 

initial_heigth_bar = driver.find_element(By.ID, "ipHb") #initial height 
initial_heigth_bar.clear()
initial_heigth_bar.send_keys(str(init_height))

time.sleep(1) # delay for 1 second .


initial_speed = driver.find_element(By.ID, "ipVb") #initial speed (Vi) 
initial_speed.clear()
initial_speed.send_keys(str(vi))

time.sleep(1) # delay for 1 second .

angle_tetha = driver.find_element(By.ID, "ipWb") #angle (theta) 
angle_tetha.clear()
angle_tetha.send_keys(str(angle))

time.sleep(1) # delay for 1 second .

# step 3 : click slow motion checkbox to optimise the visualisation 

slow_motion_cb = driver.find_element(By.ID, "cbSlow") #slow motion checkbox
slow_motion_cb.click()
time.sleep(1) # delay for 1 second .

# step 4 : click to start the simulation

start_button = wait.until(EC.element_to_be_clickable((By.ID, "bu2")))
driver.execute_script("arguments[0].scrollIntoView();", start_button)
start_button.click()


# Close the browser
# driver.quit()