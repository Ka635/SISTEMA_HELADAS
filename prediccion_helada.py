import sys
import joblib
import pandas as pd
import json

# Cargar el modelo entrenado
modelo = joblib.load("arbol_heladas.pkl")

# Leer datos de entrada desde Node-RED
input_data = sys.stdin.read()

try:
    # Convertir la entrada JSON a un diccionario
    data_dict = json.loads(input_data)

    # Crear un DataFrame con los datos recibidos
    datos_prueba = pd.DataFrame([[
        data_dict["estacion"], 
        data_dict["horario"], 
        data_dict["temperatura"], 
        data_dict["humedad"], 
        data_dict["Viento"], 
        data_dict["indice_uv"]
    ]], columns=['estacion', 'horario', 'temperatura', 'humedad', 'Viento', 'indice UV'])

    # Realizar la predicción
    prediccion = modelo.predict(datos_prueba)

    # Enviar salida a Node-RED
    if prediccion[0] == 1:
        print(json.dumps({"alerta": "⚠ ALERTA: Helada detectada", "estado": 1}))
    else:
        print(json.dumps({"alerta": "✅ No hay riesgo de helada", "estado": 0}))

except Exception as e:
    print(json.dumps({"error": str(e)}))
    