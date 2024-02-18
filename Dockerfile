FROM python:3.12.2-slim

WORKDIR /app

COPY . /app

RUN pip install -r requirements.txt
RUN mkdir "instance" && touch instance/config.py && echo 'SECRET_KEY = "'$(python -c "import secrets; print(secrets.token_hex())")'"' > instance/config.py

CMD ["python3", "run.py"]

EXPOSE 80