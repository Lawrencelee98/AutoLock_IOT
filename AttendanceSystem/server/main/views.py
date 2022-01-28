from django.http import HttpResponse, JsonResponse
from django.shortcuts import render
import json
from.models import *
from django.core.serializers import serialize
from datetime import datetime
from django.core.paginator import Paginator

# Create your views here.
def attendance(request):
    if request.method == 'POST':
        byte_data = request.body
        # print(byte_data)
        id = byte_data[:11].decode('utf-8')
        user = User.objects.filter(user_id=id).exists()
        # 如果查询到有结果，则记录到Record里
        if user:
            user = User.objects.first()
            now = datetime.now()
            date_time = now.strftime("%Y-%m-%d %H:%M:%S")
            Record.objects.create(item_name=user.user_name, item_id=user.user_id, item_date=date_time, item_create_date= now)
            response = '000'
        else:
            response = '001'
        return HttpResponse(response)

    response = '000'
    return HttpResponse(response)


'''
    item_num = models.AutoField(primary_key=True)
    item_name = models.CharField(max_length=20)
    item_id = models.CharField(max_length=50)
    item_date = models.DateTimeField(auto_now_add=True)

'''

def timelist(request):
    cli_request = json.loads(request.body)
    try:
        pageinfo = cli_request['pageinfo']
        print(pageinfo)
    except Exception:
        print("argument error")

    # 创建分页器

    records = []
    total = Record.objects.count()
    if total > 0:
        datas = Record.objects.all().order_by('-item_date')
        paginator = Paginator(datas, pageinfo['pagesize'])
        # datas = paginator.get_page(pageinfo['currentpage'])
        datas = []
        try:
            datas = paginator.page(pageinfo['currentpage'])
            print(datas)
        except Exception as error:
            print(f"page error:{error}")

        for data in datas:
            record = {"id": data.item_id, "name": data.item_name, "date": data.item_date}
            records.append(record)

    return JsonResponse({"timeList":records, "total":total}, safe=False)



