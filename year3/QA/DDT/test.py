import time

from selenium import webdriver
import pytest
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select


LATENCY = 0.1


class Tests:

    def fill_fields(self, driver):
        t = '/html/body/div[11]/div[2]/div/div[5]/form/table/tbody/tr[{}]/td[3]/input'
        for row in range(8):
            driver.find_element(By.XPATH, t.format(row + 1)).send_keys('randomString')
            time.sleep(LATENCY)

    def choose_accs(self, driver, acc):
        if acc != -1:
            check_box = '/html/body/center/div/div[2]/div/div[3]/form/table/tbody/tr[{}]/td[1]/input'
            driver.find_element(By.XPATH, check_box.format(acc + 2)).click()

    def choose_specials(self, driver, special):
        combo_box = Select(driver.find_element(By.ID, "SpecialsCombo"))
        time.sleep(LATENCY)
        combo_box.select_by_visible_text(special)

    def choose_car(self, driver, car):
        table = '/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{}]/td[1]'
        driver.find_element(By.XPATH, table.format(car + 2)).click()

    @pytest.mark.parametrize('cars', (0, 1, 2, 3, 4))
    @pytest.mark.parametrize('specials', ('- no special model -', 'Gomera', 'Jazz', 'Luxus'))
    @pytest.mark.parametrize('accs', (-1, 0, 1, 2, 3, 4, 5, 6, 7))
    def test_start(self, cars, specials, accs):
        # driver = webdriver.Firefox()
        # driver.get("") 
        self.choose_car(driver, cars)
        time.sleep(LATENCY)
        driver.find_element(By.ID, "TabbedPaneSpecialModel").click()
        self.choose_specials(driver, specials)
        time.sleep(LATENCY)
        driver.find_element(By.ID, "TabbedPaneAccessory").click()
        self.choose_accs(driver, accs)
        time.sleep(LATENCY)
        driver.find_element(By.ID, "mOrder_menu").click()
        time.sleep(LATENCY)
        driver.find_element(By.ID, "miSendOrder_menu").click()
        time.sleep(LATENCY)
        driver.find_element(By.ID, "SendPurchaseOrderDialogAccordionContactInDesc").click()
        time.sleep(LATENCY)
        self.fill_fields(driver)
        driver.find_element(By.ID, "SendOrderButton").click()

        url = driver.current_url
        driver.quit()

        assert 'confirmEN.pdf' in url
