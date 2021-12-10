Feature: Change the vehicle from the options

  Scenario: Change the vehicle via Change button
    Given Car configurator page is open
     When User open options tab
      And Choose car
      And User clears fields
      And User prints name "devyatka" and price "100"
      And Click "Change" button
     Then Car name and price appears in the table
      And Exit

    Scenario: Change the vehicle via Edit button
    Given Car configurator page is open
     When User open options tab
      And User clicks edit on chosen car
     Then Edit Vehicle window appears
      And User clears fields in window
      And User prints name "devyatka" and price "100" in window
      And Click "Ok" Button
     Then Car name and price appears in the table
      And Exit
