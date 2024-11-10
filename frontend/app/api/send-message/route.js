import { NextResponse } from "next/server";
import net from "net";

export async function POST(request) {
  try {
    // Parse the incoming JSON body
    const { message, selectedServer } = await request.json();

    // Define the proxy's TCP connection details (localhost and port 9090)
    const proxyHost = "localhost";
    const proxyPort = 9090;

    // Create a socket connection to the proxy
    const socket = new net.Socket();

    // Set up a Promise to handle the response from the proxy
    const responsePromise = new Promise((resolve, reject) => {
      socket.connect(proxyPort, proxyHost, () => {
        console.log("Connected to proxy server");

        // Send the message and selectedServer to the proxy
        const messageToSend = JSON.stringify({ message, selectedServer });
        socket.write(messageToSend);
      });

      // Listen for data from the proxy
      socket.on("data", (data) => {
        console.log("Received data from proxy: ", data.toString());
        resolve(data.toString()); // Resolve the promise with the proxy's response
      });

      // Handle errors
      socket.on("error", (error) => {
        console.error("Socket error:", error);
        reject("Error sending message to proxy");
      });

      // Handle when the socket is closed
      socket.on("close", () => {
        console.log("Connection closed");
      });
    });

    // Wait for the proxy's response
    const proxyResponse = await responsePromise;

    // Return the response from the proxy to the frontend
    return NextResponse.json({
      message: proxyResponse,
    });
  } catch (error) {
    console.error("Error communicating with proxy:", error);
    return NextResponse.json({ error: "An error occurred while communicating with the proxy" }, { status: 500 });
  }
}
