def areYouHungry(ate=False):
    if ate:
        return "No"
    else:
        return "Yes"

print("Are you hungry?", areYouHungry() )


ateSomething = True
print("Are you hungry?", areYouHungry(ateSomething) )
