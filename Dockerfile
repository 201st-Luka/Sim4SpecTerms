FROM python:3.11.5

WORKDIR /app

COPY . /app

RUN pip install -r requirements.txt
RUN python3 setup.py install
RUN mkdir "instance" && touch instance/config.py && echo 'SECRET_KEY = "'$(python -c "import secrets; print(secrets.token_hex())")'"' > instance/config.py

CMD ["python3", "run.py"]

EXPOSE 8080