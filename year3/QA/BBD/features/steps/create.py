import time
from utils import *
from behave import *
from selenium import webdriver
from selenium.webdriver.common.by import By


@given('Car configurator page is open')
def step_impl(context):
    driver = webdriver.Firefox()
    driver.get("file:///D:\Programing\HSE-NN-Studying\year3\QA\BBD\html\CarConfig.htm")
    context.driver = driver
    time.sleep(LATENCY)


@when("User open options tab")
def step_impl(context):
    driver = context.driver
    driver.find_element(By.ID, "mOptions_menu").click()
    time.sleep(LATENCY)
    driver.find_element(By.ID, "miVehicles_menu").click()
    time.sleep(LATENCY)


@step('User prints name "{name}" and price "{price}"')
def step_impl(context, name, price):
    driver = context.driver
    driver.find_element(By.ID, "VehicleName_input").send_keys(name)
    time.sleep(LATENCY)
    driver.find_element(By.ID, "VehiclePrice_input").send_keys(price)
    time.sleep(LATENCY)
    context.name = name
    context.price = price


@step('Click "New" button')
def step_impl(context):
    driver = context.driver
    driver.find_element(By.ID, "NewButton1").click()
    time.sleep(LATENCY)


@then("Car name and price appears in the table")
def step_impl(context):
    assert find_row(context, table_xpath='/html/body/div[1]/div[2]/div[1]/table')


@then("Invalid input window appears")
def step_impl(context):
    driver = context.driver
    attributes = driver.find_element(By.XPATH, '/html/body/div[13]').get_attribute('style')
    attrs_dict = attrs_to_dict(attributes)

    assert attrs_dict['display'] != 'none'


@step("Exit")
def step_impl(context):
    context.driver.quit()
