"use client";

import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Eye, Lock, Send, Server, Shield, Zap } from "lucide-react";
import { useState } from "react";
import Navbar from "@/app/UI-Components/LandingPageNavbar";

export default function Component() {
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
    <main className="h-screen w-screen text-white bg-gradient-to-br from-indigo-900 via-violet-600 to-indigo-900">
      <Navbar />

      <main className="container w-full h-full mx-auto px-4">
        <section className="min-h-screen flex items-center justify-center">
          <div className="text-center space-y-8">
            <h1 className="text-5xl font-bold mb-4">Why Choose SecureProxy?</h1>
            <div className="grid gap-8 md:grid-cols-3">
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Lock className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">Enhanced Privacy</h3>
                  <p className="text-purple-300">Your data is encrypted and your identity is protected.</p>
                </CardContent>
              </Card>
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Zap className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">Lightning Fast</h3>
                  <p className="text-purple-300">Our servers ensure quick and efficient message delivery.</p>
                </CardContent>
              </Card>
              <Card className="bg-purple-900 bg-opacity-50 border-purple-600">
                <CardContent className="flex flex-col items-center space-y-4 p-6">
                  <Eye className="w-12 h-12 text-purple-400" />
                  <h3 className="text-xl font-semibold">No Logging Policy</h3>
                  <p className="text-purple-300">We dont keep any logs of your activities or messages.</p>
                </CardContent>
              </Card>
            </div>
          </div>
        </section>
      </main>
    </main>
  );
}
