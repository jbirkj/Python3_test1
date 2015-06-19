# module for logging data somwhere


def GoogleSubmit(value1, value2, value3):
    try:
        gs = gspread.login('jesperbirkp@gmail.com', 'zxkfdgmtpslbqpzg')
    except:
        print('fail')
        sys.exit
    wks = gs.open("TempLog1").sheet1
    
    #writing to Google sheet
    values = [ datetime.datetime.now(), 'sensor', value1, value2, value3]
    wks.append_row(values)
    
    #wks.close() 


def FileLogging(v1, v2, v3):
	
