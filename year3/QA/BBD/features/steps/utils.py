import selenium
from selenium.webdriver.common.by import By

LATENCY = 0.5


def attrs_to_dict(attributes: str):
    attrs_dict = dict()
    for keyval in attributes.split(';'):
        t = keyval.split(':')
        if len(t) > 1:
            key = t[0].strip()
            val = t[1].strip()
            attrs_dict[key] = val
    return attrs_dict


def find_row(ctx, table_xpath: str) -> bool:
    row = 2
    row_xpath = table_xpath + '/tbody/tr[{}]/td[{}]'
    try:
        while True:
            n = ctx.driver.find_element(By.XPATH, row_xpath.format(row, 1)).text
            p = ctx.driver.find_element(By.XPATH, row_xpath.format(row, 2)).text.split('.')[0]
            if n == ctx.name and p == ctx.price:
                break
            row += 1
    except selenium.common.exceptions.NoSuchElementException:
        return False
    return True
