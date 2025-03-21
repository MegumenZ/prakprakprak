from flask import Flask, render_template, jsonify
import firebase_admin
from firebase_admin import credentials, db

app = Flask(__name__)

# Inisialisasi Firebase Admin SDK
cred = credentials.Certificate("project-4-5eb1b-firebase-adminsdk-fbsvc-b6f9418c5e.json")  # Path file SDK key Firebase
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://project-4-5eb1b-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Ganti dengan URL Firebase Realtime Database
})

# Referensi ke data sensor
ref = db.reference('/WaterDistance')


@app.route('/')
def index():
    """Menampilkan halaman monitoring"""
    snapshot = ref.get()  # Ambil data terbaru
    distance = snapshot if snapshot else "Data belum tersedia"
    return render_template('index.html', distance=distance)


@app.route('/data')
def get_data():
    """API untuk mengambil data terbaru dalam format JSON"""
    snapshot = ref.get()
    data = {'distance': snapshot} if snapshot else {'distance': "Data belum tersedia"}
    return jsonify(data)


# Tidak perlu app.run() karena Vercel akan menangani servernya
