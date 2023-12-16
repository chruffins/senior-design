local date = os.date('*t')
local time = os.date("*t")
print("The current date and time is " .. os.date("%A, %m %B %Y | ") .. ("%02d:%02d:%02d"):format(time.hour, time.min, time.sec))