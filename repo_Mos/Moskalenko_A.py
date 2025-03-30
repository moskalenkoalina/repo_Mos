import turtle
import time
import math


class Watch:
    def __init__(self):
        pass

    def update(self):
        raise NotImplementedError("This method should be overridden in subclasses")


class Digit:
    def __init__(self, number, x, y):
        self.number = number
        self.x = x
        self.y = y

    def draw(self):
        turtle.penup()
        turtle.goto(self.x, self.y)
        turtle.write(self.number, align="center", font=("Arial", 16, "bold"))


class Dial:
    def __init__(self, radius):
        self.radius = radius

    def draw(self):
        turtle.penup()
        turtle.goto(0, -self.radius)
        turtle.pendown()
        turtle.circle(self.radius)
        for i in range(1, 13):
            angle = math.radians(30 * i)
            x = (self.radius - 20) * math.sin(angle)
            y = (self.radius - 20) * math.cos(angle)
            digit = Digit(i, x, y)
            digit.draw()


class Hand:
    def __init__(self, length, width, color):
        self.length = length
        self.width = width
        self.color = color
        self.turtle = turtle.Turtle()
        self.turtle.penup()

    def draw(self, angle):
        self.turtle.clear()
        self.turtle.goto(0, 0)
        self.turtle.setheading(90 - angle)
        self.turtle.pendown()
        self.turtle.pensize(self.width)
        self.turtle.color(self.color)
        self.turtle.forward(self.length)
        self.turtle.penup()


class AnalogWatch(Watch):
    def __init__(self):
        super().__init__()
        self.dial = Dial(150)
        self.hour_hand = Hand(70, 6, "black")
        self.minute_hand = Hand(100, 4, "blue")
        self.second_hand = Hand(120, 2, "red")
        self.dial.draw()

    def update(self):
        turtle.tracer(0, 0)
        current_time = time.localtime()
        hours = current_time.tm_hour % 12
        minutes = current_time.tm_min
        seconds = current_time.tm_sec

        hour_angle = (hours + minutes / 60) * 30
        minute_angle = (minutes + seconds / 60) * 6
        second_angle = seconds * 6

        self.hour_hand.draw(hour_angle)
        self.minute_hand.draw(minute_angle)
        self.second_hand.draw(second_angle)

        turtle.update()
        turtle.ontimer(self.update, 1000)

    def run(self):
        turtle.speed(0)
        turtle.hideturtle()
        self.update()
        turtle.done()


class DigitalWatch(Watch):
    def __init__(self, hour_format=24):
        super().__init__()
        self.hour_format = hour_format
        self.alarm_time = None
        self.theme = "light"
        self.screen = turtle.Screen()
        self.screen.tracer(0, 0)
        self.display_turtle = turtle.Turtle()
        self.display_turtle.hideturtle()

    def set_alarm(self, hour, minute):
        self.alarm_time = (hour, minute)

    def check_alarm(self, hours, minutes):
        if self.alarm_time and (hours, minutes) == self.alarm_time:
            print("Будильник! Пора вставати!")

    def set_theme(self, theme):
        if theme in ["light", "dark"]:
            self.theme = theme
        else:
            print("Невідомий режим. Використовується стандартна світла тема.")

    def update(self):
        self.display_turtle.clear()
        current_time = time.localtime()
        hours = current_time.tm_hour
        minutes = current_time.tm_min
        seconds = current_time.tm_sec

        if self.hour_format == 12:
            period = "AM" if hours < 12 else "PM"
            hours = hours % 12 or 12
            time_str = f"{hours:02}:{minutes:02}:{seconds:02} {period}"
        else:
            time_str = f"{hours:02}:{minutes:02}:{seconds:02}"

        self.check_alarm(hours, minutes)
        self.display_turtle.penup()
        self.display_turtle.goto(0, 0)
        self.display_turtle.write(time_str, align="center", font=("Arial", 24, "bold"))
        turtle.update()
        self.screen.ontimer(self.update, 1000)

    def run(self):
        self.update()
        turtle.done()


if __name__ == "__main__":
    choice = input("Виберіть тип годинника (analog/digital): ")
    if choice == "analog":
        clock = AnalogWatch()
        clock.run()
    elif choice == "digital":
        hour_format = int(input("Оберіть формат часу (12/24): "))
        clock = DigitalWatch(hour_format)
        alarm_choice = input("Бажаєте встановити будильник? (yes/no): ")
        if alarm_choice == "yes":
            alarm_hour = int(input("Введіть годину: "))
            alarm_minute = int(input("Введіть хвилини: "))
            clock.set_alarm(alarm_hour, alarm_minute)
        theme_choice = input("Виберіть тему (light/dark): ")
        clock.set_theme(theme_choice)
        clock.run()
