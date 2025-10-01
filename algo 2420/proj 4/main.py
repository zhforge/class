from slist import SList
from course import Course

def calculate_gpa(courseList):
    sumGrades = 0
    credits = 0
    for course in courseList:
        sumGrades += course.value.grade() * course.value.credit_hr()
        credits += course.value.credit_hr()
    if credits == 0:
        return 0
    return sumGrades / credits

def is_sorted(lyst):
    for i in range(0, len(lyst)  - 1):
        if lyst[i] > lyst[i + 1]:
            return False
    return True

def main():
    a = Course(1000, 'a', 3.0, 3.0)
    b = Course(3000, 'b', 2.0, 3.5)
    c = Course(2000, 'c', 2.2, 1.8)
    courses = SList()
    courses.insert(a)
    courses.insert(b)
    courses.insert(c)

    print(is_sorted(courses))
    print(calculate_gpa(courses))
  
if __name__ == "__main__":
    main()