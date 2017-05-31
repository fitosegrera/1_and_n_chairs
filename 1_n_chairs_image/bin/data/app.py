import urllib, httplib, sys, json, random

try:
    from urllib.parse import urlparse
except ImportError:
     from urlparse import urlparse

qry = sys.argv[1] #phrase to search on bing image engine
cnt = 100 #total search results

sp = qry.split('_')
qry = ""
for word in sp:
	qry += word + " "

print qry

headers2 = {
    # Request headers
    'Ocp-Apim-Subscription-Key': '87a92e2aca3243cc8ecd03e8a64fe6a5',
}

params = urllib.urlencode({
    # Request parameters
    'q': qry,
    'count': cnt,
    'offset': '0',
    'mkt': 'en-us',
    'safeSearch': 'Moderate',
})

try:
	print "-------"
	conn = httplib.HTTPSConnection('api.cognitive.microsoft.com')
	conn.request("GET", "/bing/v5.0/images/search?%s" % params, "{body}", headers2)
	response = conn.getresponse()
	data = response.read()
	res_json2 = json.loads(data)
	results2 = json.dumps(res_json2, indent=2, sort_keys=True)
	retrying = True
	retries = 1
	while retrying:
		global retrying
		try:
			print "TRYING", retries
			rand = random.randint(0,cnt)
			url_dict = urlparse(res_json2['value'][rand]['contentUrl'])
			splitted = url_dict[4].split('&r=')
			splitted = splitted[1].split('&p=')
			new_url = urllib.unquote(splitted[0]).decode('utf8') 
			print new_url
			img_resp = urllib.urlretrieve(new_url, "data/image.jpg")
			print "IMG Downloaded"
			retrying = False
		except Exception as e:
			print e
			retries+=1
	
	conn.close()

except Exception as e:
    print(e)
