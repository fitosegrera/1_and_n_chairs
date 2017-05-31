import requests, urllib
import json

session = requests.Session()

api_url='https://southeastasia.api.cognitive.microsoft.com/vision/v1.0/analyze'

headers = {
  # Request headers.
  'Content-Type': 'application/octet-stream',

  # NOTE: Replace the "Ocp-Apim-Subscription-Key" value with a valid subscription key.
  'Ocp-Apim-Subscription-Key': 'ccd1ca5c528e477780168a1a6715cf32',
}

params = urllib.urlencode({
    'visualFeatures': 'Categories,Tags,Description,Faces,ImageType,Color,Adult',
})

query_string = '?{0}'.format(params)

url = api_url + query_string

filename = '/home/pi/openFrameworks/apps/myApps/1_n_chairs_camera/bin/data/frame.png'
f = open(filename, 'rb')
data = f.read()

resp = session.post(url, headers=headers, data=data)

res_json = json.loads(resp.content.decode('utf-8'))
results = json.dumps(res_json, indent=2, sort_keys=True)
#print results

print res_json['description']['captions'][0]['text']
