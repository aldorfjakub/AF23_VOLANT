import serial
import time
import struct

START_BYTE = 0xAA
ACK_PACKET_TYPE = 0x06
BATTERY_PACKET_TYPE = 0x03
DASH_PACKET_TYPE = 0x02

HELLO_PACKET_TYPE = 0x00

class SerialPacketReader:
    def __init__(self, port='/dev/ttyACM0', baud_rate=115200):
        self.ser = serial.Serial(port, baud_rate, timeout=1)
        self.buffer = bytearray()

    def calculate_crc(self, data):
        """Calculate CRC8 (simple XOR-based CRC)."""
        crc = 0
        for byte in data:
            crc ^= byte
        return crc

    def read_packet(self):
        while self.ser.in_waiting > 0:
            byte = self.ser.read(1)
            print(byte)
            if not self.buffer and byte[0] != START_BYTE:
                continue
            
            self.buffer.extend(byte)
            
            # Minimum packet size: start(1) + type(1) + timestamp(4) + length(1) + crc(1)
            if len(self.buffer) < 8:
                continue
            
            # Verify start byte
            if self.buffer[0] != START_BYTE:
                self.buffer.clear()
                continue
            
            # Get data length
            data_length = self.buffer[4]
            
            # Check if we have complete packet
            expected_packet_length = 8 + data_length
            if len(self.buffer) < expected_packet_length:
                continue
            
            # Extract packet components
            packet_type = self.buffer[1]
            timestamp = struct.unpack('I', self.buffer[2:6])[0]
            data = self.buffer[6:6+data_length]
            received_crc = self.buffer[-1]
            
            # Validate CRC
            packet_data = list(self.buffer[:-1])
            calculated_crc = self.calculate_crc(packet_data)
            
            # Clear buffer before returning
            self.buffer.clear()
            
            if received_crc == calculated_crc:
                return {
                    'packet_type': packet_type,
                    'timestamp': timestamp,
                    'data_length': data_length,
                    'data': list(data),
                    'crc': received_crc
                }
            else:
                print("CRC mismatch, discarding packet.")
                return None
    
        return None

    def send_ack(self):
        """Send an ACK response."""
        packet = bytearray([START_BYTE, ACK_PACKET_TYPE])
        crc = self.calculate_crc(packet)
        packet.append(crc)
        self.ser.write(packet)

    def send_battery(self):
        """Send battery data response."""
        battery_info = [
            [80, 37, 40],
            [88, 37, 24],
            [87, 38, 24],
            [20, 35, 24],
            [5, 37, 24],
            [99, 40, 24],
            [100, 40, 24],
            [45, 45, 24]
        ]
        
        packet = bytearray([START_BYTE, BATTERY_PACKET_TYPE, 0x18])
        
        # Scaling the battery info and appending to the packet
        for row in battery_info:
            for value in row:
                scaled_value = max(0, min(255, value))
                packet.append(scaled_value)
        
        crc = self.calculate_crc(packet)
        packet.append(crc)
        print(packet)
        self.ser.write(packet)
    def send_dashboard(self):
        """Send battery data response."""


        
        packet = bytearray([START_BYTE, DASH_PACKET_TYPE, 0x05])

        packet.extend(struct.pack('>H',15000))
        packet.append(84)
        packet.append(32)

        packed_byte = (1 << 4) | 0  # 0x12 (hex) or 18 (decimal)
        packet.append(packed_byte)


        
        # Scaling the battery info and appending to the packet
       
        
        crc = self.calculate_crc(packet)
        packet.append(crc)
        print(packet)
        self.ser.write(packet)

def main():
    reader = SerialPacketReader()
    print("Waiting for packet...")
    
    while True:
        packet = reader.read_packet()
        
        if packet:
            print(f"Received packet: {packet}")
            
            if packet['packet_type'] == HELLO_PACKET_TYPE:
                reader.send_ack()
                print("ACK sent!")
            elif packet['packet_type'] == BATTERY_PACKET_TYPE:
                reader.send_battery()
                print("Battery response sent")
            elif packet['packet_type'] == DASH_PACKET_TYPE:
                reader.send_dashboard()
                print("Dashboard response sent")
        
        time.sleep(0.01)  # Small delay to prevent CPU hogging

if __name__ == "__main__":
    main()