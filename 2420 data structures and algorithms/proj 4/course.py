''' Course Class for Project 4 of CS 2420 '''

class Course:
    ''' Course object '''
    def __init__(self, number = 0, name = '', credit_hr = 0, grade = 0):
        if not isinstance(number, int): raise ValueError("Input course number as a non-negative number")
        if number < 0: raise ValueError("Input course number as a non-negative number")
        if not isinstance(name, str): raise ValueError("Input course name as a string")
        if not (isinstance(credit_hr, float) or isinstance(credit_hr, int)): raise ValueError("Input credit hours as a non-negative number with a decimal")
        if credit_hr < 0: raise ValueError("Input credit hours as a non-negative number with a decimal")
        if not (isinstance(grade, float) or isinstance(grade, int)): raise ValueError("Input grade as a number between 0.0 to 4.0")
        if not (grade >= 0.0 and grade <= 4.0): raise ValueError("Input grade as a number between 0.0 to 4.0")

        self._number = number
        self._name = name
        self._credit_hr = float(credit_hr)
        self._grade = float(grade)

    def number(self):
        return self._number
    def name(self):
        return self._name
    def credit_hr(self):
        return self._credit_hr
    def grade(self):
        return self._grade
  
    def __eq__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() == cnumb
      
    def __ne__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() != cnumb
      
    def __lt__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() < cnumb
      
    def __gt__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() > cnumb
      
    def __le__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() <= cnumb
      
    def __ge__(self, other):
        cnumb = other
        if isinstance(other, Course):
            cnumb = other.number()
        return self.number() >= cnumb
      
    def __str__(self):
        return f"cs{self._number} {self._name} Grade: {self._grade} Credit Hours: {self._credit_hr}"