from flask import Flask, render_template, jsonify
import firebase_admin
from firebase_admin import credentials, db

app = Flask(__name__)

# Inisialisasi Firebase Admin SDK
cred = credentials.Certificate("project-4-5eb1b-firebase-adminsdk-fbsvc-b6f9418c5e.json")  # Gunakan raw string
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://project-4-5eb1b-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Ganti dengan URL database
})

# Referensi ke data sensor
ref = db.reference('/WaterDistance')

@app.route('/')
def index():
    """Menampilkan halaman monitoring"""
    snapshot = ref.get()  # Ambil data terbaru dari Firebase
    distance = snapshot if snapshot else "Data belum tersedia"
    return render_template('index.html', distance=distance)

@app.route('/data')
def get_data():
    """API untuk mengambil data terbaru dalam format JSON"""
    snapshot = ref.get()
    data = {'distance': snapshot} if snapshot else {'distance': "Data belum tersedia"}
    return jsonify(data)

# Untuk menjalankan secara lokal
if __name__ == "__main__":
    app.run(debug=True)
