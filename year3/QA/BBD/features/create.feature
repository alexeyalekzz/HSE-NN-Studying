Feature: Create a vehicle from the options

  Scenario: Create a vehicle
    Given Car configurator page is open
     When User open options tab
      And User prints name "devyatka" and price "100"
      And Click "New" button
     Then Car name and price appears in the table
      And Exit

  Scenario: Create vehicle with invalid price
     Given Car configurator page is open
     When User open options tab
      And User prints name "devyatka" and price " "
      And Click "New" button
     Then Invalid input window appears
      And Exit

  Scenario: Create vehicle with invalid name
     Given Car configurator page is open
     When User open options tab
      And User prints name " " and price "100"
      And Click "New" button
     Then Invalid input window appears
      And Exit