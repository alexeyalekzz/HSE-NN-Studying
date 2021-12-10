import time
from utils import *
from behave import *
from selenium.webdriver.common.by import By


@step("Choose car")
def step_impl(context):
    driver = context.driver
    row1_xpath = '/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[{}]'
    context.name = driver.find_element(By.XPATH, row1_xpath.format(1)).text
    context.price = driver.find_element(By.XPATH, row1_xpath.format(2)).text
    driver.find_element(By.XPATH, row1_xpath.format(1)).click()
    time.sleep(LATENCY)


@step('Click "Change" button')
def step_impl(context):
    context.driver.find_element(By.ID, "ChangeButton1").click()
    time.sleep(LATENCY)


@step("User clicks edit on chosen car")
def step_impl(context):
    row1_edit_xpath = '/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[3]/a[2]'
    context.driver.find_element(By.XPATH, row1_edit_xpath).click()
    time.sleep(LATENCY)


@then("Edit Vehicle window appears")
def step_impl(context):
    edit_window_xpath = '/html/body/div[3]'
    attributes = context.driver.find_element(By.XPATH, edit_window_xpath).get_attribute('style')
    attrs_dict = attrs_to_dict(attributes)

    assert attrs_dict['display'] != 'none'


@step("User clears fields")
def step_impl(context):
    context.driver.find_element(By.ID, "VehicleName_input").clear()
    context.driver.find_element(By.ID, "VehiclePrice_input").clear()
    time.sleep(LATENCY)


@step('Click "Ok" Button')
def step_impl(context):
    context.driver.find_element(By.ID, 'OkButton3').click()
    time.sleep(LATENCY)


@step('User prints name "{name}" and price "{price}" in window')
def step_impl(context, name, price):
    driver = context.driver
    driver.find_element(By.ID, "VehicleDialogEditPanelVehicleName_input").send_keys(name)
    time.sleep(LATENCY)
    driver.find_element(By.ID, "VehicleDialogEditPanelVehiclePrice_input").send_keys(price)
    time.sleep(LATENCY)
    context.name = name
    context.price = price


@step("User clears fields in window")
def step_impl(context):
    context.driver.find_element(By.ID, "VehicleDialogEditPanelVehicleName_input").clear()
    context.driver.find_element(By.ID, "VehicleDialogEditPanelVehiclePrice_input").clear()
    time.sleep(LATENCY)
