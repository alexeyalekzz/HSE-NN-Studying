Feature: Delete the vehicle from the options

  Scenario: Delete the vehicle from the delete button
    Given Car configurator page is open
     When User open options tab
      And Choose car
      And Press "Delete" button
      And Resolve alert
     Then Record deleted from table
      And Exit

  Scenario: Delete the vehicle from delete in row
    Given Car configurator page is open
     When User open options tab
      And User clicks delete on chosen car
      And Resolve alert
     Then Record deleted from table
      And Exit
