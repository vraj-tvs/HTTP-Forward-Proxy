"use client";

import Navbar from "@/app/UI-Components/Navbar";
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Box } from "@radix-ui/themes";
import { Send, Server, Shield } from "lucide-react";
import { useState } from "react";

function ServerResponse({ response }: { response: string }) {
  if (!response) {
    return null;
  }
  return (
    <CardFooter className="bg-purple-700 mt-4 rounded-2xl py-3 w-full">
      <div className="space-y-2 w-full px-3">
        <h2 className="font-semibold flex items-center gap-2 text-lg text-violet-200">
          <Server className="w-4 h-4" /> Server Response
        </h2>
        <p className="text-white text-md">{response}</p>
      </div>
    </CardFooter>
  );
}

function HeadersCard() {
  return (
    <CardHeader>
      <CardTitle className="text-4xl font-bold flex items-center gap-2">
        <Shield className="w-9 h-9" /> Forward Proxy Server
      </CardTitle>
      <CardDescription className="text-purple-300 text-lg pl-10">Secure and private message forwarding</CardDescription>
    </CardHeader>
  );
}

function TestingProxy() {
  const [selectedServer, setSelectedServer] = useState("");
  const [message, setMessage] = useState("");
  const [response, setResponse] = useState("");
  const servers = [
    { id: "server1", name: "Server 1 - Fast & Secure" },
    { id: "server2", name: "Server 2 - Ultra Private" },
    { id: "server3", name: "Server 3 - Balanced" },
  ];

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();

    setResponse(`Message sent through ${selectedServer}: "${message}"`);
    setMessage("");
  };
  return (
    <div className="flex-row">
      <Navbar />
      <div className="flex justify-center items-center">
        <Box className="bg-green-1000">
          <section className="min-h-screen flex items-center justify-center">
            <Card className="w-full bg-gradient-to-br from-indigo-950 via-purple-900 to-indigo-950 p-6">
              <HeadersCard />
              <CardContent>
                <form onSubmit={handleSubmit} className="space-y-8">
                  <div className="space-y-2 w-full">
                    <label htmlFor="server-select" className="text-lg font-medium">
                      Select Server
                    </label>
                    <Select onValueChange={setSelectedServer} required>
                      <SelectTrigger id="server-select" className="bg-purple-800 border-purple-600 text-md rounded-xl">
                        <SelectValue placeholder="Choose a server" />
                      </SelectTrigger>
                      <SelectContent className="bg-purple-800 border-purple-600 rounded-xl">
                        {servers.map((server) => (
                          <SelectItem key={server.id} value={server.id} className="text-white text-sm">
                            {server.name}
                          </SelectItem>
                        ))}
                      </SelectContent>
                    </Select>
                  </div>
                  <div className="space-y-2">
                    <label htmlFor="message" className="text-lg font-medium">
                      Your Message
                    </label>
                    <input
                      id="message"
                      value={message}
                      onChange={(e) => setMessage(e.target.value)}
                      className="bg-purple-800 border-purple-600 text-white placeholder-purple-300 rounded-xl text-md container py-2 px-4"
                      placeholder="Enter your message here"
                      required
                    />
                  </div>
                  <button
                    type="submit"
                    className="w-full bg-purple-600 bg-gradient-to-br from-purple-600 via-purple-800 to-purple-600 hover:bg-purple-400 text-white text-md rounded-xl flex justify-center items-center py-2 font-medium"
                  >
                    <Send className="w-4 h-4 mr-2" /> Send Message
                  </button>
                </form>
              </CardContent>
              <ServerResponse response={response} />
            </Card>
          </section>
        </Box>
      </div>
    </div>
  );
}

export default TestingProxy;
