FROM python:3.12.2-slim

WORKDIR /app

COPY . /app

RUN pip install -r requirements.txt

EXPOSE 80

CMD ["python3", "run.py"]
