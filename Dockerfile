FROM python:3.11.5

WORKDIR /app

COPY . /app

RUN pip install -r requirements.txt
RUN python3 setup.py build
RUN python3 setup.py install

CMD ["python3", "app.py"]

EXPOSE 80