import time
from utils import *
from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.common.alert import Alert


@step('Press "Delete" button')
def step_impl(context):
    context.driver.find_element(By.ID, "DeleteButton1").click()
    time.sleep(LATENCY)


@step("Resolve alert")
def step_impl(context):
    Alert(context.driver).accept()
    time.sleep(LATENCY)


@step("User clicks delete on chosen car")
def step_impl(context):
    driver = context.driver
    row1_xpath = '/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[{}]'
    row1_delete_xpath = '/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[3]/a[1]'
    context.name = driver.find_element(By.XPATH, row1_xpath.format(1)).text
    context.price = driver.find_element(By.XPATH, row1_xpath.format(2)).text
    driver.find_element(By.XPATH, row1_delete_xpath).click()
    time.sleep(LATENCY)


@then("Record deleted from table")
def step_impl(context):
    assert not find_row(context, table_xpath='/html/body/div[1]/div[2]/div[1]/table')
