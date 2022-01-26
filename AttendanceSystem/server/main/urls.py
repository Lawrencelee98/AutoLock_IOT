from django.urls import path
from .views import *

urlpatterns = [
    path('attendance', attendance, name='attendance'),
    path('timelist', timelist, name='timelist')
]